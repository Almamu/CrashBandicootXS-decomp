.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_801FEEC
sub_801FEEC: @ 0x0801FEEC
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _08020000 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x6c
	str r0, [r4, #0x20]
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r7, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r5, r0, #0
	ldr r1, [r5, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #9
	str r0, [r5, #0x6c]
	str r5, [r4, #0x44]
	ldr r1, [r5, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r5, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #1
	movs r6, #1
	strb r0, [r4, #0xa]
	movs r0, #0x7f
	ldrb r7, [r4, #0xc]
	ands r0, r7
	strb r0, [r4, #0xc]
	ldr r0, _08020004 @ =gUnknown_030012B4
	mov sb, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r6
	ands r0, r6
	adds r3, r4, #0
	adds r3, #0x28
	ands r0, r6
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r6
	ands r0, r6
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _08020008 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	ldr r1, _0802000C @ =gStaticData_0816B98C
	adds r0, r5, #0
	adds r0, #0x84
	str r1, [r0]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r4, [r1, #0xc]
	mov r2, r8
	ldrh r2, [r2]
	adds r4, r2, r4
	adds r0, r5, #0
	movs r1, #6
	bl sub_800C6A8
	ldr r0, [r4, #8]
	ldr r1, [r4, #0xc]
	ldr r2, [r4, #4]
	str r0, [r5, #0x3c]
	str r1, [r5, #0x40]
	str r2, [r5, #0x44]
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08020000: .4byte gUnknown_030012D0
_08020004: .4byte gUnknown_030012B4
_08020008: .4byte gUnknown_030012F0
_0802000C: .4byte gStaticData_0816B98C

	thumb_func_start sub_8020010
sub_8020010: @ 0x08020010
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r4, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r4, #0
	bl sub_8009ED0
	adds r5, r0, #0
	ldr r0, _08020128 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x96
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r5, #0x20]
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #0x19
	str r0, [r6, #0x6c]
	str r6, [r5, #0x44]
	ldr r1, [r6, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #1
	movs r7, #1
	strb r0, [r5, #0xa]
	movs r0, #0x7f
	ldrb r1, [r5, #0xc]
	ands r0, r1
	strb r0, [r5, #0xc]
	ldr r0, _0802012C @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r4, r4, #1
	adds r4, r4, r0
	ldr r2, [r1, #0xc]
	ldrh r4, [r4]
	adds r2, r4, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r7
	ands r0, r7
	adds r4, r5, #0
	adds r4, #0x28
	ands r0, r7
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	mov r8, r1
	ldrb r3, [r4]
	ands r1, r3
	orrs r1, r0
	strb r1, [r4]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r7
	ands r0, r7
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r4]
	ldr r0, _08020130 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	ldr r1, _08020134 @ =gStaticData_0816B98C
	adds r0, r6, #0
	adds r0, #0x84
	str r1, [r0]
	ldrb r2, [r4]
	lsls r0, r2, #0x1b
	movs r1, #0
	cmp r0, #0
	blt _080200FC
	movs r1, #1
_080200FC:
	ands r1, r7
	lsls r1, r1, #4
	mov r0, r8
	ands r0, r2
	orrs r0, r1
	strb r0, [r4]
	movs r0, #2
	strb r0, [r5, #0xa]
	subs r0, #0x43
	ldrb r1, [r5, #0xc]
	ands r0, r1
	strb r0, [r5, #0xc]
	adds r0, r6, #0
	movs r1, #1
	bl sub_800C6A8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08020128: .4byte gUnknown_030012D0
_0802012C: .4byte gUnknown_030012B4
_08020130: .4byte gUnknown_030012F0
_08020134: .4byte gStaticData_0816B98C

	thumb_func_start sub_8020138
sub_8020138: @ 0x08020138
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r5, r0, #0
	ldr r0, _08020258 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r7, #0xa2
	lsls r7, r7, #1
	adds r0, r0, r7
	str r0, [r5, #0x20]
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r4, r0, #0
	ldr r1, [r4, #0xc]
	movs r7, #0x18
	ldrsh r0, [r1, r7]
	adds r0, r4, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #0x1b
	str r0, [r4, #0x6c]
	str r4, [r5, #0x44]
	ldr r1, [r4, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #1
	movs r6, #1
	strb r0, [r5, #0xa]
	movs r0, #0x7f
	ldrb r3, [r5, #0xc]
	ands r0, r3
	strb r0, [r5, #0xc]
	ldr r7, _0802025C @ =gUnknown_030012B4
	mov sb, r7
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r6
	ands r0, r6
	adds r3, r5, #0
	adds r3, #0x28
	ands r0, r6
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r6
	ands r0, r6
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _08020260 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	ldr r0, _08020264 @ =gStaticData_0816B98C
	adds r2, r4, #0
	adds r2, #0x84
	str r0, [r2]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r0, [r1, #0xc]
	mov r3, r8
	ldrh r3, [r3]
	adds r0, r3, r0
	ldr r1, _08020268 @ =gStaticData_0816BACC
	str r1, [r2]
	ldr r1, [r0, #8]
	ldr r2, [r0, #4]
	ldr r0, [r0, #0xc]
	str r1, [r4, #0x30]
	str r2, [r4, #0x34]
	str r0, [r4, #0x38]
	adds r0, r4, #0
	movs r1, #4
	bl sub_800C6A8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08020258: .4byte gUnknown_030012D0
_0802025C: .4byte gUnknown_030012B4
_08020260: .4byte gUnknown_030012F0
_08020264: .4byte gStaticData_0816B98C
_08020268: .4byte gStaticData_0816BACC

	thumb_func_start sub_802026C
sub_802026C: @ 0x0802026C
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r5, r0, #0
	ldr r0, _08020394 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r7, #0x90
	lsls r7, r7, #1
	adds r0, r0, r7
	str r0, [r5, #0x20]
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r4, r0, #0
	ldr r1, [r4, #0xc]
	movs r7, #0x18
	ldrsh r0, [r1, r7]
	adds r0, r4, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #0x18
	str r0, [r4, #0x6c]
	str r4, [r5, #0x44]
	ldr r1, [r4, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #1
	movs r6, #1
	strb r0, [r5, #0xa]
	movs r0, #0x7f
	ldrb r3, [r5, #0xc]
	ands r0, r3
	strb r0, [r5, #0xc]
	ldr r7, _08020398 @ =gUnknown_030012B4
	mov sb, r7
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r6
	ands r0, r6
	adds r3, r5, #0
	adds r3, #0x28
	ands r0, r6
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r6
	ands r0, r6
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0802039C @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	ldr r0, _080203A0 @ =gStaticData_0816B98C
	adds r2, r4, #0
	adds r2, #0x84
	str r0, [r2]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r1, [r1, #0xc]
	mov r3, r8
	ldrh r3, [r3]
	adds r1, r3, r1
	ldr r0, _080203A4 @ =gStaticData_0816BAEC
	str r0, [r2]
	ldr r0, [r1, #8]
	ldr r2, [r1, #0xc]
	ldr r3, [r1, #0x10]
	str r0, [r4, #0x30]
	str r2, [r4, #0x34]
	str r3, [r4, #0x38]
	ldr r1, [r1, #4]
	adds r0, r4, #0
	bl sub_800C898
	adds r0, r4, #0
	movs r1, #0xd
	bl sub_800C6A8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08020394: .4byte gUnknown_030012D0
_08020398: .4byte gUnknown_030012B4
_0802039C: .4byte gUnknown_030012F0
_080203A0: .4byte gStaticData_0816B98C
_080203A4: .4byte gStaticData_0816BAEC

	thumb_func_start sub_80203A8
sub_80203A8: @ 0x080203A8
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	ldr r7, _080204D4 @ =0xFFD80000
	adds r2, r2, r7
	lsrs r2, r2, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r5, r0, #0
	ldr r0, _080204D8 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xae
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r5, #0x20]
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r4, r0, #0
	ldr r1, [r4, #0xc]
	movs r7, #0x18
	ldrsh r0, [r1, r7]
	adds r0, r4, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #0x1d
	str r0, [r4, #0x6c]
	str r4, [r5, #0x44]
	ldr r1, [r4, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #1
	movs r6, #1
	strb r0, [r5, #0xa]
	movs r0, #0x7f
	ldrb r3, [r5, #0xc]
	ands r0, r3
	strb r0, [r5, #0xc]
	ldr r7, _080204DC @ =gUnknown_030012B4
	mov sb, r7
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r6
	ands r0, r6
	adds r3, r5, #0
	adds r3, #0x28
	ands r0, r6
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r6
	ands r0, r6
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _080204E0 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	ldr r0, _080204E4 @ =gStaticData_0816B98C
	adds r2, r4, #0
	adds r2, #0x84
	str r0, [r2]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r1, [r1, #0xc]
	mov r3, r8
	ldrh r3, [r3]
	adds r1, r3, r1
	ldr r0, _080204E8 @ =gStaticData_0816BB4C
	str r0, [r2]
	movs r0, #0x78
	movs r2, #0x5a
	ldr r1, [r1, #0xc]
	str r0, [r4, #0x30]
	str r2, [r4, #0x34]
	str r1, [r4, #0x38]
	adds r0, r4, #0
	movs r1, #0x28
	bl sub_800C898
	adds r0, r4, #0
	movs r1, #0x12
	bl sub_800C6A8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080204D4: .4byte 0xFFD80000
_080204D8: .4byte gUnknown_030012D0
_080204DC: .4byte gUnknown_030012B4
_080204E0: .4byte gUnknown_030012F0
_080204E4: .4byte gStaticData_0816B98C
_080204E8: .4byte gStaticData_0816BB4C

	thumb_func_start sub_80204EC
sub_80204EC: @ 0x080204EC
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _08020618 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r7, #0x9c
	lsls r7, r7, #1
	adds r0, r0, r7
	str r0, [r4, #0x20]
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r7, #0x18
	ldrsh r0, [r1, r7]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #0x1a
	str r0, [r6, #0x6c]
	str r6, [r4, #0x44]
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #1
	movs r5, #1
	strb r0, [r4, #0xa]
	movs r0, #0x7f
	ldrb r3, [r4, #0xc]
	ands r0, r3
	strb r0, [r4, #0xc]
	ldr r7, _0802061C @ =gUnknown_030012B4
	mov sb, r7
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r5
	ands r0, r5
	adds r3, r4, #0
	adds r3, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r5
	ands r0, r5
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _08020620 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	ldr r0, _08020624 @ =gStaticData_0816B98C
	adds r2, r6, #0
	adds r2, #0x84
	str r0, [r2]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r1, [r1, #0xc]
	mov r3, r8
	ldrh r3, [r3]
	adds r1, r3, r1
	movs r0, #0xa
	strb r0, [r4, #0xa]
	subs r0, #0x4b
	ldrb r7, [r4, #0xc]
	ands r0, r7
	strb r0, [r4, #0xc]
	ldr r0, _08020628 @ =gStaticData_0816BB2C
	str r0, [r2]
	ldr r0, [r1, #4]
	ldr r2, [r1, #8]
	ldr r1, [r1, #0xc]
	str r0, [r6, #0x30]
	str r2, [r6, #0x34]
	str r1, [r6, #0x38]
	adds r0, r6, #0
	movs r1, #4
	bl sub_800C6A8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08020618: .4byte gUnknown_030012D0
_0802061C: .4byte gUnknown_030012B4
_08020620: .4byte gUnknown_030012F0
_08020624: .4byte gStaticData_0816B98C
_08020628: .4byte gStaticData_0816BB2C

	thumb_func_start sub_802062C
sub_802062C: @ 0x0802062C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	adds r4, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r4, #0
	bl sub_8009ED0
	adds r7, r0, #0
	ldr r0, _08020774 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r7, #0x20]
	adds r0, r7, #0
	bl sub_800815C
	adds r2, r7, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r5, #0x18
	ldrsh r0, [r1, r5]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r7, #0
	bl sub_803AD80
	movs r0, #0x17
	str r0, [r6, #0x6c]
	str r6, [r7, #0x44]
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r7, #0
	bl sub_803AD80
	movs r3, #1
	mov r8, r3
	strb r3, [r7, #0xa]
	movs r0, #0x7f
	ldrb r5, [r7, #0xc]
	ands r0, r5
	strb r0, [r7, #0xc]
	ldr r0, _08020778 @ =gUnknown_030012B4
	mov sb, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r4, r4, #1
	adds r0, r4, r0
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r1, [r2]
	lsrs r0, r1, #1
	mov r3, r8
	eors r0, r3
	ands r0, r3
	adds r3, r7, #0
	adds r3, #0x28
	mov r5, r8
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	mov sl, r1
	ldrb r5, [r3]
	ands r1, r5
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	mov r2, r8
	ands r0, r2
	ands r0, r2
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _0802077C @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r7, #0
	str r3, [sp]
	bl sub_8008E94
	ldr r0, _08020780 @ =gStaticData_0816B98C
	adds r5, r6, #0
	adds r5, #0x84
	str r0, [r5]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	adds r4, r4, r0
	ldr r0, [r1, #0xc]
	ldrh r4, [r4]
	adds r2, r4, r0
	ldr r3, [sp]
	ldrb r4, [r3]
	lsls r0, r4, #0x1b
	movs r1, #0
	cmp r0, #0
	blt _0802073A
	movs r1, #1
_0802073A:
	mov r0, r8
	ands r1, r0
	lsls r1, r1, #4
	mov r0, sl
	ands r0, r4
	orrs r0, r1
	strb r0, [r3]
	movs r1, #1
	strb r1, [r7, #0xa]
	ldr r0, _08020784 @ =gStaticData_0816BAAC
	str r0, [r5]
	ldr r0, [r2, #8]
	ldr r1, [r2, #4]
	ldr r2, [r2, #0xc]
	str r0, [r6, #0x30]
	str r1, [r6, #0x34]
	str r2, [r6, #0x38]
	adds r0, r6, #0
	movs r1, #4
	bl sub_800C6A8
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08020774: .4byte gUnknown_030012D0
_08020778: .4byte gUnknown_030012B4
_0802077C: .4byte gUnknown_030012F0
_08020780: .4byte gStaticData_0816B98C
_08020784: .4byte gStaticData_0816BAAC

	thumb_func_start sub_8020788
sub_8020788: @ 0x08020788
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r6, r0, #0
	ldr r0, _080208B4 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r7, #0x84
	lsls r7, r7, #1
	adds r0, r0, r7
	str r0, [r6, #0x20]
	adds r0, r6, #0
	bl sub_800815C
	adds r2, r6, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r5, r0, #0
	ldr r1, [r5, #0xc]
	movs r7, #0x18
	ldrsh r0, [r1, r7]
	adds r0, r5, r0
	ldr r2, [r1, #0x1c]
	adds r1, r6, #0
	bl sub_803AD80
	movs r0, #0x16
	str r0, [r5, #0x6c]
	str r5, [r6, #0x44]
	ldr r1, [r5, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r2, [r1, #0x1c]
	adds r1, r6, #0
	bl sub_803AD80
	movs r0, #1
	movs r3, #0
	mov sl, r3
	movs r4, #1
	strb r0, [r6, #0xa]
	movs r0, #0x7f
	ldrb r7, [r6, #0xc]
	ands r0, r7
	strb r0, [r6, #0xc]
	ldr r0, _080208B8 @ =gUnknown_030012B4
	mov sb, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r4
	ands r0, r4
	adds r3, r6, #0
	adds r3, #0x28
	ands r0, r4
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r4
	ands r0, r4
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _080208BC @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r6, #0
	bl sub_8008E94
	ldr r1, _080208C0 @ =gStaticData_0816B98C
	adds r0, r5, #0
	adds r0, #0x84
	str r1, [r0]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r4, [r1, #0xc]
	mov r2, r8
	ldrh r2, [r2]
	adds r4, r2, r4
	adds r0, r5, #0
	movs r1, #9
	bl sub_800C6A8
	ldr r1, [r4, #4]
	ldr r2, [r4, #8]
	ldr r3, [r4, #0xc]
	adds r0, r5, #0
	bl sub_800C860
	movs r0, #0x80
	movs r1, #0x14
	str r0, [r5, #0x3c]
	mov r3, sl
	str r3, [r5, #0x40]
	str r1, [r5, #0x44]
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080208B4: .4byte gUnknown_030012D0
_080208B8: .4byte gUnknown_030012B4
_080208BC: .4byte gUnknown_030012F0
_080208C0: .4byte gStaticData_0816B98C

	thumb_func_start sub_80208C4
sub_80208C4: @ 0x080208C4
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _080209D8 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0xf0
	str r0, [r4, #0x20]
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r7, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #0x14
	str r0, [r6, #0x6c]
	str r6, [r4, #0x44]
	ldr r1, [r6, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #1
	movs r5, #1
	strb r0, [r4, #0xa]
	movs r0, #0x7f
	ldrb r7, [r4, #0xc]
	ands r0, r7
	strb r0, [r4, #0xc]
	ldr r0, _080209DC @ =gUnknown_030012B4
	mov sb, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r5
	ands r0, r5
	adds r3, r4, #0
	adds r3, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r5
	ands r0, r5
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _080209E0 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	ldr r0, _080209E4 @ =gStaticData_0816B98C
	adds r2, r6, #0
	adds r2, #0x84
	str r0, [r2]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r4, [r1, #0xc]
	mov r3, r8
	ldrh r3, [r3]
	adds r4, r3, r4
	ldr r0, _080209E8 @ =gStaticData_0816BB0C
	str r0, [r2]
	adds r0, r6, #0
	movs r1, #2
	bl sub_800C6A8
	ldr r1, [r4, #4]
	adds r0, r6, #0
	bl sub_800C898
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080209D8: .4byte gUnknown_030012D0
_080209DC: .4byte gUnknown_030012B4
_080209E0: .4byte gUnknown_030012F0
_080209E4: .4byte gStaticData_0816B98C
_080209E8: .4byte gStaticData_0816BB0C

	thumb_func_start sub_80209EC
sub_80209EC: @ 0x080209EC
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _08020AFC @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0xfc
	str r0, [r4, #0x20]
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r7, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #0x15
	str r0, [r6, #0x6c]
	str r6, [r4, #0x44]
	ldr r1, [r6, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #1
	movs r5, #1
	strb r0, [r4, #0xa]
	movs r0, #0x7f
	ldrb r7, [r4, #0xc]
	ands r0, r7
	strb r0, [r4, #0xc]
	ldr r0, _08020B00 @ =gUnknown_030012B4
	mov sb, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r5
	ands r0, r5
	adds r3, r4, #0
	adds r3, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r5
	ands r0, r5
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _08020B04 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	ldr r1, _08020B08 @ =gStaticData_0816B98C
	adds r0, r6, #0
	adds r0, #0x84
	str r1, [r0]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r4, [r1, #0xc]
	mov r2, r8
	ldrh r2, [r2]
	adds r4, r2, r4
	adds r0, r6, #0
	movs r1, #2
	bl sub_800C6A8
	ldr r1, [r4, #4]
	adds r0, r6, #0
	bl sub_800C898
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08020AFC: .4byte gUnknown_030012D0
_08020B00: .4byte gUnknown_030012B4
_08020B04: .4byte gUnknown_030012F0
_08020B08: .4byte gStaticData_0816B98C

	thumb_func_start sub_8020B0C
sub_8020B0C: @ 0x08020B0C
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r4, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r4, #0
	bl sub_8009ED0
	adds r5, r0, #0
	ldr r0, _08020C04 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0xe4
	str r0, [r5, #0x20]
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	mov r8, r0
	ldr r1, [r0, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	add r0, r8
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #0x13
	mov r3, r8
	str r0, [r3, #0x6c]
	str r3, [r5, #0x44]
	ldr r1, [r3, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	add r0, r8
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #1
	movs r6, #1
	strb r0, [r5, #0xa]
	movs r0, #0x7f
	ldrb r3, [r5, #0xc]
	ands r0, r3
	strb r0, [r5, #0xc]
	ldr r0, _08020C08 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r4, r4, #1
	adds r4, r4, r0
	ldr r2, [r1, #0xc]
	ldrh r4, [r4]
	adds r2, r4, r2
	ldrb r4, [r2]
	lsrs r0, r4, #1
	eors r0, r6
	ands r0, r6
	adds r3, r5, #0
	adds r3, #0x28
	ands r0, r6
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r4, [r3]
	ands r1, r4
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r6
	ands r0, r6
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _08020C0C @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	ldr r0, _08020C10 @ =gStaticData_0816B98C
	mov r1, r8
	adds r1, #0x84
	str r0, [r1]
	movs r0, #7
	strb r0, [r5, #0xa]
	ldr r0, _08020C14 @ =gStaticData_0816BB0C
	str r0, [r1]
	mov r0, r8
	movs r1, #8
	bl sub_800C6A8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08020C04: .4byte gUnknown_030012D0
_08020C08: .4byte gUnknown_030012B4
_08020C0C: .4byte gUnknown_030012F0
_08020C10: .4byte gStaticData_0816B98C
_08020C14: .4byte gStaticData_0816BB0C

	thumb_func_start sub_8020C18
sub_8020C18: @ 0x08020C18
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r5, r0, #0
	ldr r0, _08020D3C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0x48
	str r0, [r5, #0x20]
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r7, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #6
	str r0, [r6, #0x6c]
	str r6, [r5, #0x44]
	ldr r1, [r6, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	movs r0, #1
	movs r4, #1
	strb r0, [r5, #0xa]
	movs r0, #0x7f
	ldrb r7, [r5, #0xc]
	ands r0, r7
	strb r0, [r5, #0xc]
	ldr r0, _08020D40 @ =gUnknown_030012B4
	mov sb, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r4
	ands r0, r4
	adds r3, r5, #0
	adds r3, #0x28
	ands r0, r4
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r4
	ands r0, r4
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _08020D44 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	ldr r1, _08020D48 @ =gStaticData_0816B98C
	adds r0, r6, #0
	adds r0, #0x84
	str r1, [r0]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r4, [r1, #0xc]
	mov r2, r8
	ldrh r2, [r2]
	adds r4, r2, r4
	movs r0, #4
	strb r0, [r5, #0xa]
	adds r0, r6, #0
	movs r1, #0xb
	bl sub_800C6A8
	ldr r1, [r4, #0x10]
	ldr r2, [r4, #0x14]
	ldr r3, [r4, #0x18]
	adds r0, r6, #0
	bl sub_800C860
	ldr r1, [r4, #4]
	ldr r2, [r4, #8]
	ldr r3, [r4, #0xc]
	adds r0, r6, #0
	bl sub_800C87C
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08020D3C: .4byte gUnknown_030012D0
_08020D40: .4byte gUnknown_030012B4
_08020D44: .4byte gUnknown_030012F0
_08020D48: .4byte gStaticData_0816B98C

	thumb_func_start sub_8020D4C
sub_8020D4C: @ 0x08020D4C
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r3
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _08020E70 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	adds r0, #0xd8
	str r0, [r4, #0x20]
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r7, [r2]
	ands r1, r7
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x8c
	bl sub_8026EDC
	bl sub_800CA74
	adds r6, r0, #0
	ldr r1, [r6, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #0x12
	str r0, [r6, #0x6c]
	str r6, [r4, #0x44]
	ldr r1, [r6, #0xc]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	movs r0, #1
	movs r5, #1
	strb r0, [r4, #0xa]
	movs r0, #0x7f
	ldrb r7, [r4, #0xc]
	ands r0, r7
	strb r0, [r4, #0xc]
	ldr r0, _08020E74 @ =gUnknown_030012B4
	mov sb, r0
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	mov r2, r8
	lsls r2, r2, #1
	mov r8, r2
	add r0, r8
	ldr r2, [r1, #0xc]
	ldrh r0, [r0]
	adds r2, r0, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r5
	ands r0, r5
	adds r3, r4, #0
	adds r3, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r3]
	ands r1, r7
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r5
	ands r0, r5
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _08020E78 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	ldr r0, _08020E7C @ =gStaticData_0816B98C
	adds r2, r6, #0
	adds r2, #0x84
	str r0, [r2]
	mov r1, sb
	ldr r0, [r1]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	add r8, r0
	ldr r1, [r1, #0xc]
	mov r3, r8
	ldrh r3, [r3]
	adds r1, r3, r1
	movs r0, #0xa
	strb r0, [r4, #0xa]
	subs r0, #0x4b
	ldrb r7, [r4, #0xc]
	ands r0, r7
	strb r0, [r4, #0xc]
	ldr r0, _08020E80 @ =gStaticData_0816BB2C
	str r0, [r2]
	ldr r0, [r1, #4]
	ldr r2, [r1, #8]
	ldr r1, [r1, #0xc]
	str r0, [r6, #0x30]
	str r2, [r6, #0x34]
	str r1, [r6, #0x38]
	adds r0, r6, #0
	movs r1, #4
	bl sub_800C6A8
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08020E70: .4byte gUnknown_030012D0
_08020E74: .4byte gUnknown_030012B4
_08020E78: .4byte gUnknown_030012F0
_08020E7C: .4byte gStaticData_0816B98C
_08020E80: .4byte gStaticData_0816BB2C

@ sub_8020E84/sub_8020F7C/sub_802107C/sub_802117C (the "trigger effect
@ type N" twin family) are reconstructed (extremely close, but not yet
@ byte-matching) as C in src/graphics/trigger_effect.c, guarded by
@ #if NON_MATCHING - this raw version is used only for the real
@ byte-matching build. See docs/matching.md, "Parked, not matched:
@ sub_8020E84/sub_8020F7C/sub_802107C/sub_802117C".
.if NON_MATCHING == 0
	thumb_func_start sub_8020E84
sub_8020E84: @ 0x08020E84
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	adds r4, r0, #0
	lsls r1, r1, #0x10
	lsrs r6, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r7, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r5, r3, #0x10
	ldr r0, _08020ECC @ =gUnknown_030012C0
	mov sb, r0
	ldr r0, [r0]
	movs r1, #1
	ldrb r2, [r0, #2]
	ands r2, r1
	mov r8, r2
	cmp r2, #0
	beq _08020EF4
	bl sub_8023278
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08020EC4
	mov r3, sb
	ldr r0, [r3]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _08020ED0
_08020EC4:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	movs r1, #0xc
	b _08020ED6
	.align 2, 0
_08020ECC: .4byte gUnknown_030012C0
_08020ED0:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	movs r1, #0xb
_08020ED6:
	str r1, [sp]
	adds r1, r6, #0
	adds r2, r7, #0
	adds r3, r5, #0
	bl sub_801A878
	adds r1, r0, #0
	ldr r0, _08020EF0 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80234E8
	b _08020F66
	.align 2, 0
_08020EF0: .4byte gUnknown_030012C0
_08020EF4:
	movs r0, #7
	mov sb, r0
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	adds r1, r6, #0
	adds r2, r7, #0
	adds r3, r5, #0
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _08020F74 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r2, sb
	strb r2, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	mov r0, r8
	strb r0, [r4, #0xa]
	ldr r0, _08020F78 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r1, [r4, #0xc]
	ands r0, r1
	strb r0, [r4, #0xc]
_08020F66:
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08020F74: .4byte gUnknown_030012D0
_08020F78: .4byte gUnknown_030012EC

	thumb_func_start sub_8020F7C
sub_8020F7C: @ 0x08020F7C
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	adds r4, r0, #0
	lsls r1, r1, #0x10
	lsrs r6, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r7, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r5, r3, #0x10
	ldr r0, _08020FCC @ =gUnknown_030012C0
	mov r8, r0
	ldr r1, [r0]
	movs r0, #2
	ldrb r2, [r1, #2]
	ands r0, r2
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	mov sb, r0
	cmp r0, #0
	beq _08020FF4
	adds r0, r1, #0
	bl sub_8023278
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08020FC2
	mov r3, r8
	ldr r0, [r3]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _08020FD0
_08020FC2:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	movs r1, #0xc
	b _08020FD6
	.align 2, 0
_08020FCC: .4byte gUnknown_030012C0
_08020FD0:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	movs r1, #3
_08020FD6:
	str r1, [sp]
	adds r1, r6, #0
	adds r2, r7, #0
	adds r3, r5, #0
	bl sub_801A878
	adds r1, r0, #0
	ldr r0, _08020FF0 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80234E8
	b _08021066
	.align 2, 0
_08020FF0: .4byte gUnknown_030012C0
_08020FF4:
	movs r0, #5
	mov r8, r0
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	adds r1, r6, #0
	adds r2, r7, #0
	adds r3, r5, #0
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _08021074 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r2, r8
	strb r2, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	mov r0, sb
	strb r0, [r4, #0xa]
	ldr r0, _08021078 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r1, [r4, #0xc]
	ands r0, r1
	strb r0, [r4, #0xc]
_08021066:
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08021074: .4byte gUnknown_030012D0
_08021078: .4byte gUnknown_030012EC

	thumb_func_start sub_802107C
sub_802107C: @ 0x0802107C
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	adds r4, r0, #0
	lsls r1, r1, #0x10
	lsrs r6, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r7, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r5, r3, #0x10
	ldr r0, _080210CC @ =gUnknown_030012C0
	mov r8, r0
	ldr r1, [r0]
	movs r0, #4
	ldrb r2, [r1, #2]
	ands r0, r2
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	mov sb, r0
	cmp r0, #0
	beq _080210F4
	adds r0, r1, #0
	bl sub_8023278
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _080210C2
	mov r3, r8
	ldr r0, [r3]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _080210D0
_080210C2:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	movs r1, #0xc
	b _080210D6
	.align 2, 0
_080210CC: .4byte gUnknown_030012C0
_080210D0:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	movs r1, #0xa
_080210D6:
	str r1, [sp]
	adds r1, r6, #0
	adds r2, r7, #0
	adds r3, r5, #0
	bl sub_801A878
	adds r1, r0, #0
	ldr r0, _080210F0 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80234E8
	b _08021166
	.align 2, 0
_080210F0: .4byte gUnknown_030012C0
_080210F4:
	movs r0, #6
	mov r8, r0
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	adds r1, r6, #0
	adds r2, r7, #0
	adds r3, r5, #0
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _08021174 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r2, r8
	strb r2, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	mov r0, sb
	strb r0, [r4, #0xa]
	ldr r0, _08021178 @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r1, [r4, #0xc]
	ands r0, r1
	strb r0, [r4, #0xc]
_08021166:
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08021174: .4byte gUnknown_030012D0
_08021178: .4byte gUnknown_030012EC

	thumb_func_start sub_802117C
sub_802117C: @ 0x0802117C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	adds r4, r0, #0
	lsls r1, r1, #0x10
	lsrs r5, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r6, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r7, r3, #0x10
	ldr r0, _080211D0 @ =gUnknown_030012C0
	mov r8, r0
	ldr r1, [r0]
	movs r2, #8
	mov sl, r2
	mov r0, sl
	ldrb r3, [r1, #2]
	ands r0, r3
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	mov sb, r0
	cmp r0, #0
	beq _080211F8
	adds r0, r1, #0
	bl sub_8023278
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _080211C8
	mov r1, r8
	ldr r0, [r1]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _080211D4
_080211C8:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	movs r1, #0xc
	b _080211DA
	.align 2, 0
_080211D0: .4byte gUnknown_030012C0
_080211D4:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	movs r1, #9
_080211DA:
	str r1, [sp]
	adds r1, r5, #0
	adds r2, r6, #0
	adds r3, r7, #0
	bl sub_801A878
	adds r1, r0, #0
	ldr r0, _080211F4 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80234E8
	b _08021266
	.align 2, 0
_080211F4: .4byte gUnknown_030012C0
_080211F8:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	adds r1, r5, #0
	adds r2, r6, #0
	adds r3, r7, #0
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _08021278 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r2, #0xc0
	lsls r2, r2, #1
	adds r0, r0, r2
	str r0, [r4, #0x20]
	adds r0, r4, #0
	adds r0, #0x2d
	mov r3, sl
	strb r3, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	mov r0, sb
	strb r0, [r4, #0xa]
	ldr r0, _0802127C @ =gUnknown_030012EC
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	movs r0, #5
	rsbs r0, r0, #0
	ldrb r1, [r4, #0xc]
	ands r0, r1
	strb r0, [r4, #0xc]
_08021266:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08021278: .4byte gUnknown_030012D0
_0802127C: .4byte gUnknown_030012EC
.endif

	thumb_func_start sub_8021280
sub_8021280: @ 0x08021280
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #0xc
	adds r6, r0, #0
	lsls r1, r1, #0x10
	lsrs r7, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	mov r8, r2
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov sb, r3
	ldr r5, _08021300 @ =gUnknown_030012C0
	ldr r0, [r5]
	bl sub_8023290
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0802130C
	ldr r0, [r5]
	bl sub_80232B8
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0802130C
	ldr r0, [r5]
	bl sub_8023324
	cmp r0, #0
	bne _0802130C
	ldr r4, _08021304 @ =gStaticData_0816C86C
	ldr r0, [r5]
	bl sub_802332C
	lsls r1, r0, #3
	adds r1, r1, r0
	lsls r1, r1, #2
	adds r4, #4
	adds r1, r1, r4
	ldr r0, [r1]
	cmp r0, #0
	bne _0802130C
	lsls r0, r6, #0x10
	lsrs r0, r0, #0x10
	adds r1, r7, #0
	mov r2, r8
	mov r3, sb
	bl sub_80071E4
	adds r4, r0, #0
	movs r1, #0x64
	movs r2, #0x64
	bl sub_80070EC
	movs r0, #0x12
	strb r0, [r4, #0xa]
	ldr r0, _08021308 @ =gUnknown_030012E8
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	b _08021376
	.align 2, 0
_08021300: .4byte gUnknown_030012C0
_08021304: .4byte gStaticData_0816C86C
_08021308: .4byte gUnknown_030012E8
_0802130C:
	ldr r0, _08021348 @ =gUnknown_030012D8
	ldr r0, [r0]
	adds r0, #0x88
	ldrb r0, [r0]
	cmp r0, #0
	bne _08021350
	lsls r0, r6, #0x10
	lsrs r0, r0, #0x10
	movs r1, #4
	str r1, [sp]
	adds r1, r7, #0
	mov r2, r8
	mov r3, sb
	bl sub_801A878
	ldr r1, [r0]
	asrs r1, r1, #8
	subs r2, r1, #2
	ldr r0, [r0, #4]
	asrs r0, r0, #8
	adds r3, r0, #0
	subs r3, #0x1e
	str r2, [sp, #4]
	str r3, [sp, #8]
	ldr r0, _0802134C @ =gUnknown_030012C0
	ldr r0, [r0]
	add r1, sp, #4
	bl sub_8023500
	b _08021376
	.align 2, 0
_08021348: .4byte gUnknown_030012D8
_0802134C: .4byte gUnknown_030012C0
_08021350:
	lsls r0, r6, #0x10
	lsrs r0, r0, #0x10
	adds r1, r7, #0
	mov r2, r8
	mov r3, sb
	bl sub_80071E4
	adds r4, r0, #0
	movs r1, #0x28
	movs r2, #0x28
	bl sub_80070EC
	movs r0, #0x12
	strb r0, [r4, #0xa]
	ldr r0, _08021384 @ =gUnknown_030012E8
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
_08021376:
	add sp, #0xc
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08021384: .4byte gUnknown_030012E8

	thumb_func_start sub_8021388
sub_8021388: @ 0x08021388
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	mov r8, r1
	mov sb, r2
	adds r4, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	mov r8, r1
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	mov sb, r2
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r4, #0
	bl sub_8009ED0
	adds r6, r0, #0
	ldr r0, _08021470 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r3, #0xa2
	lsls r3, r3, #2
	adds r0, r0, r3
	str r0, [r6, #0x20]
	adds r0, r6, #0
	bl sub_800815C
	adds r2, r6, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x10
	ldrb r1, [r6, #0xc]
	orrs r0, r1
	strb r0, [r6, #0xc]
	movs r0, #1
	movs r5, #1
	strb r0, [r6, #0xa]
	ldr r0, _08021474 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r4, r4, #1
	adds r4, r4, r0
	ldr r2, [r1, #0xc]
	ldrh r4, [r4]
	adds r2, r4, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r5
	ands r0, r5
	adds r3, r6, #0
	adds r3, #0x28
	ands r0, r5
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r4, [r3]
	ands r1, r4
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r5
	ands r0, r5
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	ldr r0, _08021478 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r6, #0
	bl sub_8008E94
	movs r0, #0x30
	bl sub_8026EDC
	mov r1, r8
	mov r2, sb
	bl sub_801A838
	adds r4, r0, #0
	str r4, [r6, #0x44]
	ldr r1, [r4, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r2, [r1, #0x1c]
	adds r1, r6, #0
	bl sub_803AD80
	ldr r0, _0802147C @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8023318
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08021470: .4byte gUnknown_030012D0
_08021474: .4byte gUnknown_030012B4
_08021478: .4byte gUnknown_030012F0
_0802147C: .4byte gUnknown_030012C0

	thumb_func_start sub_8021480
sub_8021480: @ 0x08021480
	push {r4, r5, r6, r7, lr}
	adds r4, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r4, #0
	bl sub_8009ED0
	adds r5, r0, #0
	ldr r0, _0802154C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xa5
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r5, #0x20]
	adds r0, r5, #0
	bl sub_800815C
	adds r2, r5, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x4c
	bl sub_8026EDC
	bl sub_80189EC
	adds r6, r0, #0
	str r6, [r5, #0x44]
	ldr r1, [r6, #0xc]
	movs r7, #0x18
	ldrsh r0, [r1, r7]
	adds r0, r6, r0
	ldr r2, [r1, #0x1c]
	adds r1, r5, #0
	bl sub_803AD80
	ldr r0, _08021550 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r4, r4, #1
	adds r4, r4, r0
	ldr r3, [r1, #0xc]
	ldrh r4, [r4]
	adds r3, r4, r3
	ldrb r1, [r3]
	lsrs r0, r1, #1
	movs r2, #1
	eors r0, r2
	ands r0, r2
	adds r4, r5, #0
	adds r4, #0x28
	ands r0, r2
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r7, [r4]
	ands r1, r7
	orrs r1, r0
	strb r1, [r4]
	ldrb r3, [r3]
	lsrs r0, r3, #2
	ands r0, r2
	ands r0, r2
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r4]
	movs r0, #0x10
	ldrb r1, [r5, #0xc]
	orrs r0, r1
	strb r0, [r5, #0xc]
	ldr r0, _08021554 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r5, #0
	bl sub_8008E94
	ldr r0, _08021558 @ =gUnknown_030012C0
	ldr r0, [r0]
	adds r1, r6, #0
	bl sub_8023318
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802154C: .4byte gUnknown_030012D0
_08021550: .4byte gUnknown_030012B4
_08021554: .4byte gUnknown_030012F0
_08021558: .4byte gUnknown_030012C0

	thumb_func_start sub_802155C
sub_802155C: @ 0x0802155C
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	adds r5, r3, #0
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r3, r5, #0
	bl sub_8009ED0
	adds r4, r0, #0
	ldr r0, _08021658 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x9f
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x20]
	movs r0, #1
	adds r1, r4, #0
	adds r1, #0x2d
	movs r2, #0
	mov sb, r2
	movs r6, #1
	strb r0, [r1]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x24
	bl sub_8026EDC
	bl sub_80197DC
	mov r8, r0
	str r0, [r4, #0x44]
	ldr r1, [r0, #0xc]
	movs r2, #0x18
	ldrsh r0, [r1, r2]
	add r0, r8
	ldr r2, [r1, #0x1c]
	adds r1, r4, #0
	bl sub_803AD80
	ldr r0, _0802165C @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r1, [r0]
	ldr r0, [r1, #8]
	lsls r5, r5, #1
	adds r5, r5, r0
	ldr r2, [r1, #0xc]
	ldrh r5, [r5]
	adds r2, r5, r2
	ldrb r3, [r2]
	lsrs r0, r3, #1
	eors r0, r6
	ands r0, r6
	adds r3, r4, #0
	adds r3, #0x28
	ands r0, r6
	lsls r0, r0, #4
	movs r1, #0x11
	rsbs r1, r1, #0
	ldrb r5, [r3]
	ands r1, r5
	orrs r1, r0
	strb r1, [r3]
	ldrb r2, [r2]
	lsrs r0, r2, #2
	ands r0, r6
	ands r0, r6
	lsls r0, r0, #5
	movs r2, #0x21
	rsbs r2, r2, #0
	ands r1, r2
	orrs r1, r0
	strb r1, [r3]
	movs r0, #0x10
	ldrb r1, [r4, #0xc]
	orrs r0, r1
	strb r0, [r4, #0xc]
	ldr r0, _08021660 @ =gUnknown_030012F4
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	adds r4, #0x2c
	mov r2, sb
	strb r2, [r4]
	ldr r0, _08021664 @ =gUnknown_030012C0
	ldr r0, [r0]
	mov r1, r8
	bl sub_8023318
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08021658: .4byte gUnknown_030012D0
_0802165C: .4byte gUnknown_030012B4
_08021660: .4byte gUnknown_030012F4
_08021664: .4byte gUnknown_030012C0

	thumb_func_start sub_8021668
sub_8021668: @ 0x08021668
	push {r4, r5, r6, lr}
	adds r4, r1, #0
	adds r5, r2, #0
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_8009ED0
	adds r6, r0, #0
	ldr r0, _0802173C @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0xb4
	lsls r1, r1, #1
	adds r0, r0, r1
	str r0, [r6, #0x20]
	lsls r4, r4, #8
	str r4, [r6]
	lsls r5, r5, #8
	str r5, [r6, #4]
	movs r0, #0
	adds r1, r6, #0
	adds r1, #0x2d
	strb r0, [r1]
	adds r0, r6, #0
	bl sub_80087C0
	adds r0, r6, #0
	bl sub_80087B4
	adds r0, r6, #0
	movs r1, #0
	bl sub_800872C
	ldr r0, _08021740 @ =gUnknown_030012B8
	ldr r0, [r0]
	ldr r1, [r6, #0x20]
	ldr r1, [r1]
	ldrb r1, [r1, #0x14]
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	adds r2, r6, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	subs r2, #1
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r1, [r2]
	ands r0, r1
	movs r1, #0x21
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r2]
	movs r0, #0x24
	bl sub_8026EDC
	bl sub_8017FE8
	str r0, [r6, #0x44]
	ldr r2, [r0, #0xc]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	ldr r2, [r2, #0x1c]
	adds r1, r6, #0
	bl sub_803AD80
	movs r0, #1
	strb r0, [r6, #0xa]
	movs r0, #0x7f
	ldrb r1, [r6, #0xc]
	ands r0, r1
	movs r1, #5
	rsbs r1, r1, #0
	ands r0, r1
	subs r1, #0x3c
	ands r0, r1
	movs r1, #0x10
	orrs r0, r1
	strb r0, [r6, #0xc]
	ldr r0, _08021744 @ =gUnknown_030012F0
	ldr r0, [r0]
	adds r1, r6, #0
	bl sub_8008E94
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0802173C: .4byte gUnknown_030012D0
_08021740: .4byte gUnknown_030012B8
_08021744: .4byte gUnknown_030012F0

	thumb_func_start sub_8021748
sub_8021748: @ 0x08021748
	push {r4, r5, lr}
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _080217C8 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x87
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x20]
	movs r5, #0
	adds r0, r4, #0
	adds r0, #0x2d
	strb r5, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x7f
	ldrb r1, [r4, #0xc]
	ands r0, r1
	movs r1, #5
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r4, #0xc]
	strb r5, [r4, #0xa]
	ldr r0, _080217CC @ =gUnknown_030012F8
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080217C8: .4byte gUnknown_030012D0
_080217CC: .4byte gUnknown_030012F8

	thumb_func_start sub_80217D0
sub_80217D0: @ 0x080217D0
	push {r4, lr}
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _08021834 @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x87
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x20]
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x7f
	ldrb r1, [r4, #0xc]
	ands r0, r1
	movs r1, #5
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r4, #0xc]
	movs r0, #0
	strb r0, [r4, #0xa]
	ldr r0, _08021838 @ =gUnknown_030012F8
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08021834: .4byte gUnknown_030012D0
_08021838: .4byte gUnknown_030012F8

	thumb_func_start sub_802183C
sub_802183C: @ 0x0802183C
	push {r4, r5, lr}
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_8008434
	adds r4, r0, #0
	ldr r0, _080218BC @ =gUnknown_030012D0
	ldr r0, [r0]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x84
	lsls r1, r1, #2
	adds r0, r0, r1
	str r0, [r4, #0x20]
	movs r5, #0
	adds r0, r4, #0
	adds r0, #0x2d
	strb r5, [r0]
	adds r0, r4, #0
	bl sub_80087C0
	adds r0, r4, #0
	bl sub_80087B4
	adds r0, r4, #0
	movs r1, #0
	bl sub_800872C
	adds r0, r4, #0
	bl sub_800815C
	adds r2, r4, #0
	adds r2, #0x29
	movs r1, #0xf
	ands r0, r1
	movs r1, #0x10
	rsbs r1, r1, #0
	ldrb r3, [r2]
	ands r1, r3
	orrs r1, r0
	strb r1, [r2]
	movs r0, #0x7f
	ldrb r1, [r4, #0xc]
	ands r0, r1
	movs r1, #5
	rsbs r1, r1, #0
	ands r0, r1
	strb r0, [r4, #0xc]
	strb r5, [r4, #0xa]
	ldr r0, _080218C0 @ =gUnknown_030012F8
	ldr r0, [r0]
	adds r1, r4, #0
	bl sub_8008E94
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080218BC: .4byte gUnknown_030012D0
_080218C0: .4byte gUnknown_030012F8

	thumb_func_start sub_80218C4
sub_80218C4: @ 0x080218C4
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #8
	str r4, [sp]
	bl sub_801A878
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_80218E8
sub_80218E8: @ 0x080218E8
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #6
	str r4, [sp]
	bl sub_801A878
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_802190C
sub_802190C: @ 0x0802190C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r5, r0, #0
	lsls r1, r1, #0x10
	lsrs r6, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r7, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	mov r8, r3
	ldr r4, _08021944 @ =gUnknown_030012C0
	ldr r0, [r4]
	bl sub_80232A0
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _0802193C
	ldr r0, [r4]
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _08021948
_0802193C:
	lsls r0, r5, #0x10
	lsrs r0, r0, #0x10
	movs r1, #7
	b _0802194E
	.align 2, 0
_08021944: .4byte gUnknown_030012C0
_08021948:
	lsls r0, r5, #0x10
	lsrs r0, r0, #0x10
	movs r1, #5
_0802194E:
	str r1, [sp]
	adds r1, r6, #0
	adds r2, r7, #0
	mov r3, r8
	bl sub_801A878
	adds r1, r0, #0
	ldr r0, _08021970 @ =gUnknown_030012C0
	ldr r0, [r0]
	bl sub_80234F4
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08021970: .4byte gUnknown_030012C0

	thumb_func_start sub_8021974
sub_8021974: @ 0x08021974
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #2
	str r4, [sp]
	bl sub_801A878
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021998
sub_8021998: @ 0x08021998
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #1
	str r4, [sp]
	bl sub_801A878
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_80219BC
sub_80219BC: @ 0x080219BC
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0
	str r4, [sp]
	bl sub_801A878
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_80219E0
sub_80219E0: @ 0x080219E0
	push {lr}
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	bl sub_801B984
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start nullsub_21
nullsub_21: @ 0x080219FC
	bx lr
	.align 2, 0

	thumb_func_start sub_8021A00
sub_8021A00: @ 0x08021A00
	push {r4, r5, lr}
	adds r4, r1, #0
	adds r5, r2, #0
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	lsls r5, r5, #0x10
	lsrs r5, r5, #0x10
	movs r0, #0x28
	bl sub_8026EDC
	bl sub_800CB40
	adds r1, r0, #0
	movs r2, #0
	ldr r0, _08021A44 @ =sub_801F680
	str r0, [r1, #0x1c]
	movs r0, #0x78
	str r0, [r1, #0x20]
	str r2, [r1, #0x24]
	lsls r4, r4, #8
	str r4, [r1]
	lsls r5, r5, #8
	str r5, [r1, #4]
	movs r0, #0x10
	ldrb r2, [r1, #0xc]
	orrs r0, r2
	strb r0, [r1, #0xc]
	ldr r0, _08021A48 @ =gUnknown_030012E8
	ldr r0, [r0]
	bl sub_8008E94
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08021A44: .4byte sub_801F680
_08021A48: .4byte gUnknown_030012E8

	thumb_func_start sub_8021A4C
sub_8021A4C: @ 0x08021A4C
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0x12
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021A70
sub_8021A70: @ 0x08021A70
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0x11
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021A94
sub_8021A94: @ 0x08021A94
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0x10
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021AB8
sub_8021AB8: @ 0x08021AB8
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0xf
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021ADC
sub_8021ADC: @ 0x08021ADC
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0xe
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021B00
sub_8021B00: @ 0x08021B00
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0xd
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021B24
sub_8021B24: @ 0x08021B24
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0xc
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021B48
sub_8021B48: @ 0x08021B48
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0xb
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021B6C
sub_8021B6C: @ 0x08021B6C
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #0xa
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021B90
sub_8021B90: @ 0x08021B90
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #9
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021BB4
sub_8021BB4: @ 0x08021BB4
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #8
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

	thumb_func_start sub_8021BD8
sub_8021BD8: @ 0x08021BD8
	push {r4, lr}
	sub sp, #4
	lsls r1, r1, #0x10
	lsrs r1, r1, #0x10
	lsls r2, r2, #0x10
	lsrs r2, r2, #0x10
	lsls r3, r3, #0x10
	lsrs r3, r3, #0x10
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r4, #7
	str r4, [sp]
	bl sub_800FF0C
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0

