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
