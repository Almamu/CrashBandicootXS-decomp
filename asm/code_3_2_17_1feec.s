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
