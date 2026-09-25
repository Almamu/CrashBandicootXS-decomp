.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_80266BC
sub_80266BC: @ 0x080266BC
	push {r4, r5, lr}
	sub sp, #4
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r0, [r5, #4]
	ldrb r1, [r0, #0x18]
	cmp r1, #0
	bne _080266D8
	ldr r0, [r0, #0x14]
	str r0, [r4, #0x24]
	adds r0, r4, #0
	adds r0, #0x28
	strb r1, [r0]
	b _080266F6
_080266D8:
	ldr r0, [r0, #0x14]
	ldr r0, [r0]
	lsrs r0, r0, #8
	bl sub_8026EC0
	adds r1, r0, #0
	str r1, [r4, #0x24]
	ldr r0, [r5, #4]
	ldr r0, [r0, #0x14]
	bl LoadTaggedAsset
	adds r1, r4, #0
	adds r1, #0x28
	movs r0, #1
	strb r0, [r1]
_080266F6:
	ldr r0, [r4, #0x10]
	ldr r1, [r5, #4]
	ldr r1, [r1, #0xc]
	bl sub_80260D4
	ldr r0, [r4, #0x20]
	ldr r1, [r5, #4]
	ldr r1, [r1, #0x10]
	bl sub_80254F8
	ldr r1, [r4, #0x10]
	ldr r0, [r1, #0x10]
	subs r0, #0xf0
	str r0, [r4]
	ldr r0, [r1, #0x14]
	subs r0, #0xa0
	str r0, [r4, #4]
	bl sub_80015D0
	ldr r0, [r4, #0x14]
	ldr r1, [r5, #4]
	ldr r1, [r1]
	bl sub_80260D4
	ldr r0, [r4, #0x14]
	adds r0, #0x28
	ldrb r0, [r0]
	cmp r0, #0
	beq _08026734
	bl sub_80015C0
_08026734:
	ldr r0, [r4, #0x18]
	ldr r1, [r5, #4]
	ldr r1, [r1, #4]
	bl sub_80260D4
	ldr r0, [r4, #0x18]
	adds r0, #0x28
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802674C
	bl sub_80015B0
_0802674C:
	ldr r0, [r4, #0x1c]
	ldr r1, [r5, #4]
	ldr r1, [r1, #8]
	bl sub_80260D4
	ldr r0, [r4, #0x1c]
	adds r0, #0x28
	ldrb r0, [r0]
	cmp r0, #0
	beq _08026764
	bl sub_80015A0
_08026764:
	ldr r0, _08026794 @ =gUnknown_030012B4
	ldr r0, [r0]
	ldr r2, [r5, #4]
	ldr r1, [r2, #0x1c]
	ldr r2, [r2, #0x20]
	movs r4, #0
	str r4, [sp]
	movs r3, #0
	bl sub_80255D4
	ldr r1, _08026798 @ =0x040000D4
	ldr r0, [r5]
	str r0, [r1]
	movs r2, #0xa0
	lsls r2, r2, #0x13
	str r2, [r1, #4]
	ldr r0, _0802679C @ =0x80000100
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	strh r4, [r2]
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08026794: .4byte gUnknown_030012B4
_08026798: .4byte 0x040000D4
_0802679C: .4byte 0x80000100

	thumb_func_start sub_80267A0
sub_80267A0: @ 0x080267A0
	push {r4, lr}
	adds r4, r0, #0
	movs r0, #0x60
	bl sub_8026EDC
	movs r1, #0
	bl sub_8026448
	str r0, [r4, #0x10]
	ldr r0, _08026808 @ =0x00001064
	bl sub_8026EDC
	bl nullsub_4
	str r0, [r4, #0x20]
	movs r0, #0x5c
	bl sub_8026EDC
	movs r1, #1
	bl sub_8025D74
	str r0, [r4, #0x14]
	movs r0, #0x5c
	bl sub_8026EDC
	movs r1, #2
	bl sub_8025D74
	str r0, [r4, #0x18]
	movs r0, #0x5c
	bl sub_8026EDC
	movs r1, #3
	bl sub_8025D74
	str r0, [r4, #0x1c]
	adds r0, r4, #0
	adds r0, #0x29
	movs r1, #0
	strb r1, [r0]
	str r1, [r4, #0x24]
	subs r0, #1
	strb r1, [r0]
	adds r0, #3
	strb r1, [r0]
	subs r0, #1
	strb r1, [r0]
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08026808: .4byte 0x00001064

	thumb_func_start sub_802680C
sub_802680C: @ 0x0802680C
	push {r4, r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	cmp r0, #0
	bne _08026820
	ldr r0, [r4, #0x24]
	cmp r0, #0
	beq _0802682A
_08026820:
	ldr r0, [r4, #0x24]
	cmp r0, #0
	beq _0802682A
	bl sub_8026EB4
_0802682A:
	ldr r2, [r4, #0x10]
	cmp r2, #0
	beq _08026840
	ldr r1, [r2, #0x30]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_08026840:
	ldr r0, [r4, #0x20]
	cmp r0, #0
	beq _0802684C
	movs r1, #3
	bl sub_8025444
_0802684C:
	ldr r2, [r4, #0x14]
	cmp r2, #0
	beq _08026862
	ldr r1, [r2, #0x30]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_08026862:
	ldr r2, [r4, #0x18]
	cmp r2, #0
	beq _08026878
	ldr r1, [r2, #0x30]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_08026878:
	ldr r2, [r4, #0x1c]
	cmp r2, #0
	beq _0802688E
	ldr r1, [r2, #0x30]
	movs r3, #8
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0xc]
	movs r1, #3
	bl sub_803AD80
_0802688E:
	ldr r1, _080268A8 @ =gUnknown_0300084C
	movs r0, #0
	str r0, [r1]
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _080268A2
	adds r0, r4, #0
	bl sub_8026ED0
_080268A2:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080268A8: .4byte gUnknown_0300084C

	thumb_func_start sub_80268AC
sub_80268AC: @ 0x080268AC
	push {r4, lr}
	ldr r4, _080268CC @ =gUnknown_0300084C
	ldr r0, [r4]
	cmp r0, #0
	bne _080268C2
	movs r0, #0x2c
	bl sub_8026EDC
	bl sub_80267A0
	str r0, [r4]
_080268C2:
	ldr r0, [r4]
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_080268CC: .4byte gUnknown_0300084C

	thumb_func_start sub_80268D0
sub_80268D0: @ 0x080268D0
	cmp r1, #0
	bge _080268D6
	movs r1, #0
_080268D6:
	cmp r2, #0
	bge _080268DC
	movs r2, #0
_080268DC:
	asrs r1, r1, #8
	asrs r2, r2, #8
	ldr r3, [r0]
	cmp r3, r1
	ble _080268E8
	adds r3, r1, #0
_080268E8:
	str r3, [r0, #8]
	ldr r1, [r0, #4]
	cmp r1, r2
	ble _080268F2
	adds r1, r2, #0
_080268F2:
	str r1, [r0, #0xc]
	bx lr
	.align 2, 0

	thumb_func_start sub_80268F8
sub_80268F8: @ 0x080268F8
	push {r4, r5, lr}
	adds r5, r0, #0
	ldr r0, [r5, #0x10]
	bl sub_8025F24
	movs r4, #0
_08026904:
	lsls r1, r4, #2
	adds r0, r5, #0
	adds r0, #0x14
	adds r0, r0, r1
	ldr r1, [r0]
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802691E
	adds r0, r1, #0
	bl sub_8025F24
_0802691E:
	adds r4, #1
	cmp r4, #2
	ble _08026904
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_802692C
sub_802692C: @ 0x0802692C
	push {r4, r5, lr}
	sub sp, #8
	adds r5, r0, #0
	ldr r0, [r5, #0x10]
	ldr r2, [r0, #0x30]
	movs r3, #0x18
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	adds r1, r5, #0
	adds r1, #8
	ldr r2, [r2, #0x1c]
	bl sub_803AD80
	ldr r1, [r5, #0x10]
	ldr r0, [r1]
	str r0, [sp]
	ldr r0, [r1, #4]
	str r0, [sp, #4]
	movs r4, #0
_08026952:
	lsls r1, r4, #2
	adds r0, r5, #0
	adds r0, #0x14
	adds r0, r0, r1
	ldr r2, [r0]
	adds r0, r2, #0
	adds r0, #0x28
	ldrb r0, [r0]
	cmp r0, #0
	beq _08026976
	ldr r1, [r2, #0x30]
	movs r3, #0x18
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0x1c]
	mov r1, sp
	bl sub_803AD80
_08026976:
	adds r4, #1
	cmp r4, #2
	ble _08026952
	add sp, #8
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_8026984
sub_8026984: @ 0x08026984
	push {r4, r5, lr}
	sub sp, #8
	adds r5, r0, #0
	ldr r0, [r5, #0x10]
	ldr r2, [r0, #0x30]
	movs r3, #0x10
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	adds r1, r5, #0
	adds r1, #8
	ldr r2, [r2, #0x14]
	bl sub_803AD80
	ldr r1, [r5, #0x10]
	ldr r0, [r1]
	str r0, [sp]
	ldr r0, [r1, #4]
	str r0, [sp, #4]
	movs r4, #0
_080269AA:
	lsls r1, r4, #2
	adds r0, r5, #0
	adds r0, #0x14
	adds r0, r0, r1
	ldr r2, [r0]
	adds r0, r2, #0
	adds r0, #0x28
	ldrb r0, [r0]
	cmp r0, #0
	beq _080269CE
	ldr r1, [r2, #0x30]
	movs r3, #0x10
	ldrsh r0, [r1, r3]
	adds r0, r2, r0
	ldr r2, [r1, #0x14]
	mov r1, sp
	bl sub_803AD80
_080269CE:
	adds r4, #1
	cmp r4, #2
	ble _080269AA
	add sp, #8
	pop {r4, r5}
	pop {r0}
	bx r0

	thumb_func_start sub_80269DC
sub_80269DC: @ 0x080269DC
	push {r4, lr}
	movs r4, #1
	cmp r1, #0
	beq _080269F0
	ldr r0, [r2]
	cmp r0, #0
	beq _080269F0
	cmp r3, #0
	beq _080269F0
	movs r4, #0
_080269F0:
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1

	thumb_func_start sub_80269F8
sub_80269F8: @ 0x080269F8
	push {r4, lr}
	movs r4, #1
	cmp r1, #0
	beq _08026A0C
	ldr r0, [r2]
	cmp r0, #0
	beq _08026A0C
	cmp r3, #0
	beq _08026A0C
	movs r4, #0
_08026A0C:
	adds r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1

	thumb_func_start sub_8026A14
sub_8026A14: @ 0x08026A14
	movs r0, #0
	bx lr

	thumb_func_start sub_8026A18
sub_8026A18: @ 0x08026A18
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xc
	mov sl, r0
	adds r7, r1, #0
	mov r8, r3
	movs r0, #0
	mov sb, r0
	add r0, sp, #4
	mov r1, sb
	strb r1, [r0]
	ldr r6, [r7, #4]
	ldr r4, [r7]
	adds r2, r4, r2
	subs r5, r2, #1
	asrs r4, r4, #3
	asrs r6, r6, #3
	asrs r5, r5, #3
	movs r0, #1
	rsbs r0, r0, #0
	cmp r4, r0
	bne _08026A4C
	movs r4, #0
_08026A4C:
	mov r2, sl
	ldr r0, [r2, #0x20]
	ldr r0, [r0, #0x10]
	cmp r5, r0
	bne _08026A58
	subs r5, #1
_08026A58:
	mov r0, sl
	adds r0, #0x2a
	str r0, [sp, #8]
	cmp r4, r5
	bgt _08026A88
_08026A62:
	mov r1, sl
	ldr r0, [r1, #0x20]
	add r2, sp, #4
	str r2, [sp]
	adds r1, r4, #0
	adds r2, r6, #0
	ldr r3, [sp, #0x2c]
	bl sub_8025130
	cmp r0, #0
	beq _08026A7C
	movs r0, #1
	mov sb, r0
_08026A7C:
	adds r4, #1
	cmp r4, r5
	bgt _08026A88
	mov r1, sb
	cmp r1, #0
	beq _08026A62
_08026A88:
	mov r2, sb
	cmp r2, #0
	beq _08026ABE
	ldr r0, [sp, #0x2c]
	cmp r0, #0
	beq _08026AAE
	cmp r0, #2
	bne _08026ABE
	ldr r0, [r7, #4]
	movs r1, #7
	ands r0, r1
	movs r1, #8
	subs r1, r1, r0
	lsls r1, r1, #8
	mov r2, r8
	ldr r0, [r2]
	adds r0, r0, r1
	str r0, [r2]
	b _08026ABE
_08026AAE:
	ldr r0, [r7, #4]
	movs r1, #7
	ands r0, r1
	lsls r0, r0, #8
	mov r2, r8
	ldr r1, [r2]
	subs r1, r1, r0
	str r1, [r2]
_08026ABE:
	ldr r1, [sp, #8]
	ldrb r0, [r1]
	cmp r0, #0
	beq _08026AD4
	add r0, sp, #4
	ldrb r1, [r0]
	cmp r1, #0
	beq _08026AD4
	mov r0, sl
	adds r0, #0x29
	strb r1, [r0]
_08026AD4:
	mov r0, sb
	add sp, #0xc
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8026AE8
sub_8026AE8: @ 0x08026AE8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xc
	mov sl, r0
	adds r7, r1, #0
	mov r8, r3
	movs r0, #0
	mov sb, r0
	add r0, sp, #4
	mov r1, sb
	strb r1, [r0]
	ldr r4, [r7, #4]
	ldr r6, [r7]
	adds r2, r4, r2
	subs r5, r2, #1
	asrs r6, r6, #3
	asrs r4, r4, #3
	asrs r5, r5, #3
	movs r0, #1
	rsbs r0, r0, #0
	cmp r4, r0
	bne _08026B1C
	movs r4, #0
_08026B1C:
	mov r1, sl
	ldr r0, [r1, #0x20]
	ldr r0, [r0, #0x14]
	cmp r5, r0
	bne _08026B28
	subs r5, #1
_08026B28:
	mov r0, sl
	adds r0, #0x2a
	str r0, [sp, #8]
	cmp r4, r5
	bgt _08026B58
_08026B32:
	mov r1, sl
	ldr r0, [r1, #0x20]
	add r1, sp, #4
	str r1, [sp]
	adds r1, r6, #0
	adds r2, r4, #0
	ldr r3, [sp, #0x2c]
	bl sub_8025130
	cmp r0, #0
	beq _08026B4C
	movs r0, #1
	mov sb, r0
_08026B4C:
	adds r4, #1
	cmp r4, r5
	bgt _08026B58
	mov r1, sb
	cmp r1, #0
	beq _08026B32
_08026B58:
	mov r0, sb
	cmp r0, #0
	beq _08026B96
	ldr r1, [sp, #0x2c]
	cmp r1, #1
	beq _08026B82
	cmp r1, #3
	bne _08026B96
	mov r0, r8
	ldr r2, [r0]
	adds r2, #1
	ldr r1, [r7]
	movs r0, #7
	ands r1, r0
	movs r0, #8
	subs r0, r0, r1
	lsls r0, r0, #8
	adds r2, r2, r0
	mov r1, r8
	str r2, [r1]
	b _08026B96
_08026B82:
	mov r1, r8
	ldr r0, [r1]
	subs r0, #1
	ldr r1, [r7]
	movs r2, #7
	ands r1, r2
	lsls r1, r1, #8
	subs r0, r0, r1
	mov r1, r8
	str r0, [r1]
_08026B96:
	ldr r1, [sp, #8]
	ldrb r0, [r1]
	cmp r0, #0
	beq _08026BAC
	add r0, sp, #4
	ldrb r1, [r0]
	cmp r1, #0
	beq _08026BAC
	mov r0, sl
	adds r0, #0x29
	strb r1, [r0]
_08026BAC:
	mov r0, sb
	add sp, #0xc
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8026BC0
sub_8026BC0: @ 0x08026BC0
	push {r4, lr}
	sub sp, #0xc
	adds r4, r0, #0
	movs r3, #0
	add r0, sp, #4
	strb r3, [r0]
	str r3, [sp, #8]
	asrs r3, r1, #3
	asrs r2, r2, #3
	cmp r3, #0
	bge _08026BD8
	movs r3, #0
_08026BD8:
	cmp r2, #0
	bge _08026BDE
	movs r2, #0
_08026BDE:
	ldr r0, [r4, #0x20]
	add r1, sp, #8
	str r1, [sp]
	adds r1, r3, #0
	add r3, sp, #4
	bl sub_8025460
	add r0, sp, #4
	ldrb r0, [r0]
	add sp, #0xc
	pop {r4}
	pop {r1}
	bx r1

	thumb_func_start sub_8026BF8
sub_8026BF8: @ 0x08026BF8
	push {r4, r5, r6, lr}
	adds r4, r1, #0
	adds r6, r2, #0
	ldr r1, [r4]
	asrs r1, r1, #3
	ldr r2, [r4, #4]
	asrs r5, r2, #3
	ldr r0, [r0, #0x20]
	adds r2, r5, #0
	bl sub_80250BC
	adds r3, r0, #0
	cmp r3, #0
	bne _08026C18
	movs r0, #0
	b _08026C36
_08026C18:
	ldr r2, [r4, #4]
	ldr r0, [r4]
	movs r1, #7
	ands r0, r1
	adds r0, r3, r0
	movs r1, #0
	ldrsb r1, [r0, r1]
	lsls r0, r5, #3
	adds r0, r0, r1
	subs r0, r0, r2
	lsls r0, r0, #8
	ldr r1, [r6]
	adds r1, r1, r0
	str r1, [r6]
	movs r0, #1
_08026C36:
	pop {r4, r5, r6}
	pop {r1}
	bx r1

	thumb_func_start sub_8026C3C
sub_8026C3C: @ 0x08026C3C
	push {r4, r5, r6, lr}
	sub sp, #8
	adds r4, r1, #0
	adds r6, r2, #0
	ldr r1, [r4]
	asrs r1, r1, #3
	ldr r2, [r4, #4]
	asrs r5, r2, #3
	ldr r0, [r0, #0x20]
	add r2, sp, #4
	str r2, [sp]
	adds r2, r5, #0
	movs r3, #0
	bl sub_8025228
	lsls r0, r0, #0x18
	asrs r2, r0, #0x18
	cmp r2, #0
	bge _08026C66
	movs r0, #0
	b _08026C78
_08026C66:
	ldr r0, [r4, #4]
	lsls r1, r5, #3
	adds r1, r1, r2
	subs r1, r1, r0
	lsls r1, r1, #8
	ldr r0, [r6]
	adds r0, r0, r1
	str r0, [r6]
	movs r0, #1
_08026C78:
	add sp, #8
	pop {r4, r5, r6}
	pop {r1}
	bx r1

	thumb_func_start sub_8026C80
sub_8026C80: @ 0x08026C80
	cmp r1, #0
	beq _08026C86
	ldr r0, [r2]
_08026C86:
	movs r0, #0
	bx lr
	.align 2, 0

	thumb_func_start sub_8026C8C
sub_8026C8C: @ 0x08026C8C
	movs r0, #0
	bx lr

	thumb_func_start sub_8026C90
sub_8026C90: @ 0x08026C90
	push {r4, r5, r6, lr}
	adds r1, r0, #0
	ldr r0, [r1, #0x10]
	ldr r2, [r0]
	ldr r3, [r0, #4]
	adds r0, #0x24
	ldrb r5, [r0]
	cmp r5, #0
	beq _08026D5C
	movs r0, #4
	ands r0, r5
	cmp r0, #0
	beq _08026CC0
	ldr r4, [r1, #0xc]
	ldr r0, _08026CB8 @ =0xFFFFE556
	cmp r4, r0
	ble _08026CC0
	ldr r6, _08026CBC @ =0xFFFFFF00
	b _08026CD4
	.align 2, 0
_08026CB8: .4byte 0xFFFFE556
_08026CBC: .4byte 0xFFFFFF00
_08026CC0:
	movs r0, #8
	ands r0, r5
	cmp r0, #0
	beq _08026CD8
	ldr r4, [r1, #0xc]
	ldr r0, _08026CEC @ =0x00001AA9
	cmp r4, r0
	bgt _08026CD8
	movs r6, #0x80
	lsls r6, r6, #1
_08026CD4:
	adds r0, r4, r6
	str r0, [r1, #0xc]
_08026CD8:
	movs r0, #2
	ands r0, r5
	cmp r0, #0
	beq _08026CF8
	ldr r4, [r1, #8]
	ldr r0, _08026CF0 @ =0xFFFFD800
	cmp r4, r0
	ble _08026CF8
	ldr r6, _08026CF4 @ =0xFFFFFF00
	b _08026D0C
	.align 2, 0
_08026CEC: .4byte 0x00001AA9
_08026CF0: .4byte 0xFFFFD800
_08026CF4: .4byte 0xFFFFFF00
_08026CF8:
	movs r0, #1
	ands r0, r5
	cmp r0, #0
	beq _08026D10
	ldr r4, [r1, #8]
	ldr r0, _08026D24 @ =0x000027FF
	cmp r4, r0
	bgt _08026D10
	movs r6, #0x80
	lsls r6, r6, #1
_08026D0C:
	adds r0, r4, r6
	str r0, [r1, #8]
_08026D10:
	movs r0, #3
	ands r0, r5
	cmp r0, #0
	bne _08026D38
	ldr r0, [r1, #8]
	cmp r0, #0
	ble _08026D2C
	ldr r4, _08026D28 @ =0xFFFFFF00
	adds r0, r0, r4
	b _08026D36
	.align 2, 0
_08026D24: .4byte 0x000027FF
_08026D28: .4byte 0xFFFFFF00
_08026D2C:
	cmp r0, #0
	bge _08026D38
	movs r6, #0x80
	lsls r6, r6, #1
	adds r0, r0, r6
_08026D36:
	str r0, [r1, #8]
_08026D38:
	movs r0, #0xc
	ands r5, r0
	cmp r5, #0
	bne _08026D5C
	ldr r0, [r1, #0xc]
	cmp r0, #0
	ble _08026D50
	ldr r4, _08026D4C @ =0xFFFFFF00
	adds r0, r0, r4
	b _08026D5A
	.align 2, 0
_08026D4C: .4byte 0xFFFFFF00
_08026D50:
	cmp r0, #0
	bge _08026D5C
	movs r6, #0x80
	lsls r6, r6, #1
	adds r0, r0, r6
_08026D5A:
	str r0, [r1, #0xc]
_08026D5C:
	ldr r0, [r1, #8]
	adds r2, r2, r0
	ldr r0, [r1, #0xc]
	adds r3, r3, r0
	ldr r4, [r1]
	subs r0, r2, r4
	cmp r0, #0
	bge _08026D6E
	adds r0, #3
_08026D6E:
	asrs r0, r0, #2
	adds r0, r4, r0
	str r0, [r1]
	ldr r4, [r1, #4]
	subs r0, r3, r4
	cmp r0, #0
	bge _08026D7E
	adds r0, #3
_08026D7E:
	asrs r0, r0, #2
	adds r0, r4, r0
	str r0, [r1, #4]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8026D8C
sub_8026D8C: @ 0x08026D8C
	push {r4, r5, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x10]
	ldr r2, [r0]
	ldr r3, [r0, #4]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _08026DB4
	ldr r1, [r4, #8]
	ldr r0, _08026DAC @ =0xFFFFED8A
	cmp r1, r0
	ble _08026DC4
	ldr r5, _08026DB0 @ =0xFFFFFF00
	b _08026DC0
	.align 2, 0
_08026DAC: .4byte 0xFFFFED8A
_08026DB0: .4byte 0xFFFFFF00
_08026DB4:
	ldr r1, [r4, #8]
	ldr r0, _08026DF4 @ =0x00001275
	cmp r1, r0
	bgt _08026DC4
	movs r5, #0x80
	lsls r5, r5, #1
_08026DC0:
	adds r0, r1, r5
	str r0, [r4, #8]
_08026DC4:
	ldr r1, _08026DF8 @ =0xFFFFF000
	str r1, [r4, #0xc]
	ldr r0, [r4, #8]
	adds r2, r2, r0
	adds r3, r3, r1
	ldr r1, [r4]
	subs r0, r2, r1
	cmp r0, #0
	bge _08026DD8
	adds r0, #3
_08026DD8:
	asrs r0, r0, #2
	adds r0, r1, r0
	str r0, [r4]
	ldr r1, [r4, #4]
	subs r0, r3, r1
	cmp r0, #0
	bge _08026DE8
	adds r0, #3
_08026DE8:
	asrs r0, r0, #2
	adds r0, r1, r0
	str r0, [r4, #4]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_08026DF4: .4byte 0x00001275
_08026DF8: .4byte 0xFFFFF000

	thumb_func_start sub_8026DFC
sub_8026DFC: @ 0x08026DFC
	push {lr}
	adds r3, r0, #0
	ldr r1, [r3, #0x10]
	ldr r0, [r1]
	str r0, [r3]
	ldr r0, [r1, #4]
	str r0, [r3, #4]
	ldr r0, [r3, #0x14]
	cmp r0, #1
	bne _08026E34
	adds r0, r1, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _08026E24
	ldr r0, _08026E20 @ =0xFFFFED8A
	b _08026E26
	.align 2, 0
_08026E20: .4byte 0xFFFFED8A
_08026E24:
	ldr r0, _08026E2C @ =0x00001276
_08026E26:
	str r0, [r3, #8]
	ldr r0, _08026E30 @ =0xFFFFF000
	b _08026E38
	.align 2, 0
_08026E2C: .4byte 0x00001276
_08026E30: .4byte 0xFFFFF000
_08026E34:
	movs r0, #0
	str r0, [r3, #8]
_08026E38:
	str r0, [r3, #0xc]
	ldr r1, [r3]
	ldr r0, [r3, #8]
	adds r1, r1, r0
	str r1, [r3]
	ldr r2, [r3, #4]
	ldr r0, [r3, #0xc]
	adds r2, r2, r0
	str r2, [r3, #4]
	ldr r0, _08026E60 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r3, _08026E64 @ =0xFFFF8800
	adds r1, r1, r3
	ldr r3, _08026E68 @ =0xFFFFB000
	adds r2, r2, r3
	bl sub_80268D0
	pop {r0}
	bx r0
	.align 2, 0
_08026E60: .4byte gUnknown_03001308
_08026E64: .4byte 0xFFFF8800
_08026E68: .4byte 0xFFFFB000

	thumb_func_start sub_8026E6C
sub_8026E6C: @ 0x08026E6C
	push {r4, lr}
	adds r4, r0, #0
	ldr r0, [r4, #0x14]
	cmp r0, #2
	beq _08026E86
	cmp r0, #2
	bgt _08026E8C
	cmp r0, #1
	bne _08026E8C
	adds r0, r4, #0
	bl sub_8026D8C
	b _08026E8C
_08026E86:
	adds r0, r4, #0
	bl sub_8026C90
_08026E8C:
	ldr r0, _08026EA8 @ =gUnknown_03001308
	ldr r0, [r0]
	ldr r1, [r4]
	ldr r2, _08026EAC @ =0xFFFF8800
	adds r1, r1, r2
	ldr r2, [r4, #4]
	ldr r3, _08026EB0 @ =0xFFFFB000
	adds r2, r2, r3
	bl sub_80268D0
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08026EA8: .4byte gUnknown_03001308
_08026EAC: .4byte 0xFFFF8800
_08026EB0: .4byte 0xFFFFB000

	thumb_func_start sub_8026EB4
sub_8026EB4: @ 0x08026EB4
	push {lr}
	bl mem_free
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8026EC0
sub_8026EC0: @ 0x08026EC0
	push {lr}
	movs r1, #0x80
	lsls r1, r1, #0x17
	bl mem_alloc
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start sub_8026ED0
sub_8026ED0: @ 0x08026ED0
	push {lr}
	bl mem_free
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_8026EDC
sub_8026EDC: @ 0x08026EDC
	push {lr}
	movs r1, #0x80
	lsls r1, r1, #0x17
	bl mem_alloc
	pop {r1}
	bx r1
	.align 2, 0

