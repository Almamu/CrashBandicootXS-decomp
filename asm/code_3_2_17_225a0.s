.include "asm/macros.inc"

.syntax unified
.arm

@ UpdateGameFrame - the main per-frame game-loop driver, part of the
@ UpdateGameFrame-MainLoop cluster (docs/rom_map.md). Left fully raw:
@ its callees in this same GitHub issue #34 chunk (sub_8022BF0,
@ sub_8022CA0, sub_8022D50, sub_8022EA8, sub_8022F2C, and the trivial
@ self+0x80/0x84/0x88/0xac/0xc0/+2-flags accessors) are now matched or
@ parked, but UpdateGameFrame itself is a ~730-instruction jump-table
@ state machine (level-load loop, per-frame dispatch on player state,
@ end-of-frame housekeeping) whose every branch/local isn't understood
@ with the precision a byte-exact reconstruction needs - see
@ docs/matching.md's entry for this chunk.
	thumb_func_start UpdateGameFrame
UpdateGameFrame: @ 0x080225A0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x1c
	adds r6, r0, #0
	movs r4, #0
	str r4, [r6, #0x68]
	bl sub_80231E4
	adds r0, r6, #0
	bl sub_80231DC
	adds r0, r6, #0
	bl sub_80231D4
	adds r0, r6, #0
	movs r1, #5
	bl sub_8023120
	adds r0, r6, #0
	movs r1, #5
	bl sub_8023118
	adds r0, r6, #0
	movs r1, #5
	bl sub_8023110
	adds r0, r6, #0
	adds r0, #0xc4
	str r4, [r0]
	adds r0, #0x1c
	strb r4, [r0]
	mov r0, sp
	strh r4, [r0]
	ldr r0, _08022634 @ =0x040000D4
	mov r1, sp
	str r1, [r0]
	str r6, [r0, #4]
	ldr r1, _08022638 @ =0x81000034
	str r1, [r0, #8]
	ldr r0, [r0, #8]
	movs r2, #0xa6
	lsls r2, r2, #1
	adds r0, r6, r2
	adds r1, r6, #0
	movs r2, #0x68
	bl sub_800014C
	ldr r0, _0802263C @ =gUnknown_030012C4
	str r6, [r0]
	str r4, [r6, #0x78]
_0802260A:
	movs r0, #0x88
	lsls r0, r0, #2
	bl sub_8026EDC
	bl LoadLevelGraphics
	adds r4, r0, #0
	bl sub_8035E14
	adds r5, r0, #0
	cmp r4, #0
	beq _0802262A
	adds r0, r4, #0
	movs r1, #3
	bl sub_8036154
_0802262A:
	cmp r5, #2
	bne _08022640
	bl sub_80354BC
	b _0802260A
	.align 2, 0
_08022634: .4byte 0x040000D4
_08022638: .4byte 0x81000034
_0802263C: .4byte gUnknown_030012C4
_08022640:
	cmp r5, #0
	beq _08022656
	bl sub_8004D4C
	movs r0, #1
	movs r1, #0
	bl sub_800300C
	bl sub_8004D20
	b _08022660
_08022656:
	ldr r0, _08022688 @ =gUnknown_030012C0
	ldr r0, [r0]
	movs r1, #2
	bl sub_8022468
_08022660:
	adds r7, r6, #0
	adds r7, #0xc4
	adds r3, r6, #0
	adds r3, #0xe0
	str r3, [sp, #0x14]
	movs r0, #0xc8
	adds r0, r0, r6
	mov sl, r0
	adds r1, r6, #0
	adds r1, #0xe4
	str r1, [sp, #0x18]
	adds r2, r6, #0
	adds r2, #0xcc
	str r2, [sp, #0x10]
	subs r3, #0x24
	str r3, [sp, #0xc]
	adds r0, r6, #0
	adds r0, #0xac
	str r0, [sp, #8]
	b _0802281C
	.align 2, 0
_08022688: .4byte gUnknown_030012C0
_0802268C:
	mov r1, r8
	cmp r1, #2
	bne _0802269E
	ldr r0, [sp, #4]
	ldr r1, [r6, #0x78]
	cmp r0, r1
	ble _0802269C
	adds r0, r1, #0
_0802269C:
	str r0, [r6, #0x78]
_0802269E:
	mov r2, r8
	cmp r2, #0
	beq _080226A6
	b _0802281C
_080226A6:
	ldr r0, [r7]
	subs r0, #0x14
	cmp r0, #4
	bhi _0802278A
	lsls r0, r0, #2
	ldr r1, _080226B8 @ =_080226BC
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080226B8: .4byte _080226BC
_080226BC: @ jump table
	.4byte _080226D0 @ case 0
	.4byte _080226F4 @ case 1
	.4byte _08022718 @ case 2
	.4byte _0802273C @ case 3
	.4byte _0802277C @ case 4
_080226D0:
	adds r0, r6, #0
	bl sub_80231BC
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _080227A4
	bl sub_801D41C
	adds r0, r6, #0
	bl sub_8023190
	bl sub_80067D4
	adds r0, r6, #0
	movs r1, #4
	bl sub_8022468
	b _080227A4
_080226F4:
	adds r0, r6, #0
	bl sub_80231CC
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _080227A4
	bl sub_801D41C
	adds r0, r6, #0
	bl sub_80231A8
	bl sub_80067C4
	adds r0, r6, #0
	movs r1, #5
	bl sub_8022468
	b _080227A4
_08022718:
	adds r0, r6, #0
	bl sub_80231B4
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _080227A4
	bl sub_801D41C
	adds r0, r6, #0
	bl sub_8023184
	bl sub_80067B4
	adds r0, r6, #0
	movs r1, #6
	bl sub_8022468
	b _080227A4
_0802273C:
	adds r0, r6, #0
	bl sub_80231C4
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08022756
	bl sub_801D41C
	adds r0, r6, #0
	bl sub_802319C
	bl sub_80067A4
_08022756:
	adds r0, r6, #0
	bl sub_800697C
	cmp r0, #0x63
	ble _08022776
	adds r0, r6, #0
	movs r1, #8
	bl sub_8022468
	ldr r0, [r7]
	adds r0, #1
	str r0, [r7]
	movs r0, #0
	mov r3, sl
	str r0, [r3]
	b _0802287E
_08022776:
	adds r0, r6, #0
	movs r1, #0xa
	b _08022780
_0802277C:
	adds r0, r6, #0
	movs r1, #9
_08022780:
	bl sub_8022468
	bl sub_80354BC
	b _080227A4
_0802278A:
	adds r0, r6, #0
	adds r0, #0xdc
	ldr r0, [r0]
	ldr r0, [r0, #8]
	cmp r0, #3
	bne _080227A4
	adds r0, r6, #0
	bl sub_8023404
	movs r1, #1
	ldrb r2, [r0]
	orrs r1, r2
	strb r1, [r0]
_080227A4:
	adds r0, r6, #0
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802280E
	adds r2, r6, #0
	adds r2, #0x98
	adds r0, r6, #0
	adds r0, #0x94
	ldr r1, [r0]
	lsls r0, r1, #2
	adds r0, r0, r1
	lsls r0, r0, #1
	ldr r2, [r2]
	adds r2, r2, r0
	adds r0, r6, #0
	adds r0, #0x90
	ldr r0, [r0]
	lsls r1, r0, #2
	adds r1, r1, r0
	lsls r0, r1, #4
	subs r0, r0, r1
	lsls r0, r0, #3
	adds r4, r2, r0
	ldr r0, _08022870 @ =0x00001FFF
	cmp r4, r0
	bls _080227DC
	adds r4, r0, #0
_080227DC:
	adds r0, r6, #0
	bl sub_8023404
	ldr r0, [r0]
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x13
	cmp r4, r0
	blo _080227FC
	adds r0, r6, #0
	bl sub_8023404
	ldr r1, _08022874 @ =0x0000FFF8
	ldrh r0, [r0]
	ands r1, r0
	cmp r1, #0
	bne _0802280E
_080227FC:
	adds r0, r6, #0
	bl sub_8023404
	lsls r2, r4, #3
	movs r1, #7
	ldrh r3, [r0]
	ands r1, r3
	orrs r1, r2
	strh r1, [r0]
_0802280E:
	movs r1, #0xa6
	lsls r1, r1, #1
	adds r0, r6, r1
	adds r1, r6, #0
	movs r2, #0x68
	bl sub_800014C
_0802281C:
	ldr r0, [r7]
	cmp r0, #0x17
	ble _08022824
	movs r0, #0x17
_08022824:
	str r0, [r7]
	adds r0, r7, #0
	bl sub_801BAF0
	adds r4, r0, #0
	lsls r4, r4, #0x18
	lsrs r4, r4, #0x18
	movs r2, #0xa6
	lsls r2, r2, #1
	adds r1, r6, r2
	adds r0, r6, #0
	movs r2, #0x68
	bl sub_800014C
	ldr r0, [sp, #0x18]
	adds r1, r6, #0
	movs r2, #0x68
	bl sub_800014C
	cmp r4, #0
	beq _08022878
	bl sub_8004D4C
	movs r0, #0
	movs r1, #0
	bl sub_800300C
	adds r4, r0, #0
	lsls r4, r4, #0x18
	lsrs r4, r4, #0x18
	bl sub_8004D20
	cmp r4, #0
	beq _0802281C
	movs r0, #0
	ldr r3, [sp, #0x14]
	strb r0, [r3]
	b _0802281C
	.align 2, 0
_08022870: .4byte 0x00001FFF
_08022874: .4byte 0x0000FFF8
_08022878:
	adds r0, r6, #0
	bl sub_80232D8
_0802287E:
	movs r5, #0
	mov r0, sl
	str r5, [r0]
	ldr r1, [sp, #0x10]
	str r5, [r1]
	ldr r2, [sp, #0x14]
	strb r5, [r2]
	movs r3, #1
	mov r8, r3
	ldr r0, [r7]
	bl sub_8024278
	ldr r1, [sp, #0xc]
	str r0, [r1]
	ldr r4, _080228F4 @ =gUnknown_03001318
	movs r0, #0x68
	bl sub_8026EDC
	bl sub_8027138
	str r0, [r4]
	ldr r2, [sp, #0xc]
	ldr r1, [r2]
	bl sub_8028568
	adds r0, r6, #0
	bl sub_80232A8
	adds r0, r6, #0
	bl sub_80232C0
	adds r0, r6, #0
	bl sub_8023280
	adds r0, r6, #0
	bl sub_8023298
	adds r0, r6, #0
	bl sub_80231D4
	adds r0, r6, #0
	bl sub_80232D0
	ldr r3, [sp, #8]
	str r5, [r3]
	ldr r4, _080228F8 @ =gUnknown_030012B4
	ldr r0, [r4]
	str r5, [r0]
	ldr r0, [r6, #0x78]
	str r0, [sp, #4]
	adds r0, r6, #0
	movs r1, #0
	bl sub_8022CA0
	adds r0, r6, #0
	bl sub_8023304
	mov sb, r4
	b _08022ABE
	.align 2, 0
_080228F4: .4byte gUnknown_03001318
_080228F8: .4byte gUnknown_030012B4
_080228FC:
	ldr r0, _08022940 @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006EA8
	ldr r0, _08022944 @ =gUnknown_03001318
	ldr r0, [r0]
	movs r1, #0
	bl sub_802732C
	ldr r0, _08022948 @ =gUnknown_0300082C
	str r5, [r0]
	adds r0, r6, #0
	movs r1, #0
	bl sub_8023318
	adds r0, r7, #0
	bl sub_8024498
	movs r0, #0xc0
	lsls r0, r0, #0x18
	bl mem_free_bytes
	adds r0, r6, #0
	adds r0, #0xdc
	ldr r0, [r0]
	ldr r1, [r0, #8]
	cmp r1, #0
	blt _08022976
	cmp r1, #2
	ble _0802294C
	cmp r1, #3
	beq _08022956
	b _08022976
	.align 2, 0
_08022940: .4byte gUnknown_030012B8
_08022944: .4byte gUnknown_03001318
_08022948: .4byte gUnknown_0300082C
_0802294C:
	adds r0, r7, #0
	bl sub_802375C
	mov r8, r0
	b _08022976
_08022956:
	ldrh r0, [r0, #0x10]
	bl InitActorCategory
	mov r8, r0
	cmp r0, #0
	bne _08022976
	bl sub_8029730
	adds r1, r0, #0
	adds r0, r6, #0
	bl sub_8023140
	adds r0, r6, #0
	movs r1, #0
	bl sub_8022CA0
_08022976:
	ldr r0, _08022AB0 @ =gUnknown_030012BC
	ldr r0, [r0]
	bl sub_80019CC
	ldr r0, [r6, #0x78]
	movs r1, #2
	cmp r0, #1
	bgt _08022988
	adds r1, r0, #0
_08022988:
	adds r0, r6, #0
	bl sub_80231EC
	movs r0, #0xc0
	lsls r0, r0, #0x18
	bl mem_free_bytes
	adds r0, r7, #0
	bl sub_8024404
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _080229D6
	adds r0, r6, #0
	bl sub_80232B8
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _080229D6
	ldr r4, _08022AB4 @ =gUnknown_030012B4
	ldr r0, [r4]
	cmp r0, #0
	beq _080229BC
	movs r1, #3
	bl sub_8025A44
_080229BC:
	movs r1, #0xda
	lsls r1, r1, #1
	adds r0, r6, r1
	ldr r0, [r0]
	str r0, [r4]
	movs r1, #0
	mov r2, r8
	cmp r2, #0
	bne _080229D0
	movs r1, #1
_080229D0:
	adds r0, r6, #0
	bl sub_8022BF0
_080229D6:
	adds r0, r7, #0
	bl sub_80243E0
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08022A16
	adds r0, r6, #0
	bl sub_8023290
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08022A16
	ldr r4, _08022AB4 @ =gUnknown_030012B4
	ldr r0, [r4]
	cmp r0, #0
	beq _080229FC
	movs r1, #3
	bl sub_8025A44
_080229FC:
	movs r3, #0xda
	lsls r3, r3, #1
	adds r0, r6, r3
	ldr r0, [r0]
	str r0, [r4]
	movs r1, #0
	mov r0, r8
	cmp r0, #0
	bne _08022A10
	movs r1, #1
_08022A10:
	adds r0, r6, #0
	bl sub_80235E4
_08022A16:
	mov r1, r8
	cmp r1, #2
	bne _08022A1E
	b _08022BAE
_08022A1E:
	cmp r1, #1
	bne _08022A2E
	adds r0, r6, #0
	bl sub_803AFEC
	cmp r0, #0
	bge _08022A2E
	b _08022BAE
_08022A2E:
	adds r0, r6, #0
	adds r0, #0x8c
	ldrb r0, [r0]
	cmp r0, #0
	beq _08022A5C
	mov r2, r8
	cmp r2, #1
	bne _08022A5C
	ldr r3, [sp, #8]
	str r5, [r3]
	adds r0, r6, #0
	adds r0, #0xd0
	strb r5, [r0]
	mov r0, sl
	str r5, [r0]
	ldr r1, [sp, #0x10]
	str r5, [r1]
	adds r0, r6, #0
	bl sub_8023304
	adds r0, r6, #0
	bl sub_80232D8
_08022A5C:
	mov r2, r8
	cmp r2, #0
	bne _08022AB8
	adds r0, r7, #0
	bl sub_8024404
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08022ABE
	adds r0, r6, #0
	bl sub_80232B8
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08022ABE
	adds r0, r7, #0
	bl sub_80243E0
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08022ABE
	adds r0, r6, #0
	bl sub_8023290
	lsls r0, r0, #0x18
	lsrs r4, r0, #0x18
	cmp r4, #0
	bne _08022ABE
	adds r0, r7, #0
	bl sub_80244F0
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08022AA2
	b _08022BAE
_08022AA2:
	adds r0, r6, #0
	bl sub_8023304
	mov r3, sb
	ldr r0, [r3]
	str r4, [r0]
	b _08022ABE
	.align 2, 0
_08022AB0: .4byte gUnknown_030012BC
_08022AB4: .4byte gUnknown_030012B4
_08022AB8:
	adds r0, r6, #0
	bl sub_8023548
_08022ABE:
	movs r0, #0xdc
	lsls r0, r0, #1
	adds r1, r6, r0
	movs r2, #0xde
	lsls r2, r2, #1
	adds r0, r6, r2
	str r5, [r0]
	str r5, [r1]
	adds r0, r6, #0
	bl sub_80232B8
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _08022AE6
	adds r0, r6, #0
	bl sub_8023290
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08022BA0
_08022AE6:
	movs r3, #0xda
	lsls r3, r3, #1
	adds r1, r6, r3
	mov r2, sb
	ldr r0, [r2]
	str r0, [r1]
	movs r0, #0x81
	lsls r0, r0, #3
	bl sub_8026EDC
	bl sub_8025A5C
	mov r3, sb
	str r0, [r3]
	adds r0, r6, #0
	bl sub_80232B8
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08022B64
	adds r0, r6, #0
	bl sub_802325C
	adds r1, r6, #0
	adds r1, #0xb0
	str r0, [r1]
	adds r0, r6, #0
	bl sub_803AFEC
	adds r1, r6, #0
	adds r1, #0xb8
	str r0, [r1]
	adds r0, r6, #0
	bl sub_8023414
	adds r1, r6, #0
	adds r1, #0xb4
	str r0, [r1]
	adds r0, r6, #0
	bl sub_80231DC
	str r5, [r6, #0x74]
	adds r0, r6, #0
	bl sub_80231D4
	adds r0, r7, #0
	bl sub_8024540
	ldr r0, _08022B60 @ =gUnknown_03001318
	ldr r4, [r0]
	adds r0, r6, #0
	adds r0, #0xdc
	ldr r0, [r0]
	bl sub_8024464
	adds r1, r0, #0
	adds r0, r4, #0
	bl sub_8028568
	b _08022B92
	.align 2, 0
_08022B60: .4byte gUnknown_03001318
_08022B64:
	adds r0, r6, #0
	bl sub_8023414
	adds r1, r6, #0
	adds r1, #0xb4
	str r0, [r1]
	adds r0, r6, #0
	bl sub_80231D4
	adds r0, r7, #0
	bl sub_8024524
	ldr r0, _08022B9C @ =gUnknown_03001318
	ldr r4, [r0]
	adds r0, r6, #0
	adds r0, #0xdc
	ldr r0, [r0]
	bl sub_8024464
	adds r1, r0, #0
	adds r0, r4, #0
	bl sub_8028568
_08022B92:
	adds r0, r6, #0
	bl sub_8023304
	b _080228FC
	.align 2, 0
_08022B9C: .4byte gUnknown_03001318
_08022BA0:
	adds r0, r7, #0
	bl sub_802455C
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08022BAE
	b _080228FC
_08022BAE:
	ldr r0, _08022BDC @ =gUnknown_03001318
	ldr r0, [r0]
	cmp r0, #0
	beq _08022BBC
	movs r1, #3
	bl sub_8028574
_08022BBC:
	adds r0, r6, #0
	bl sub_803AFEC
	cmp r0, #0
	blt _08022BC8
	b _0802268C
_08022BC8:
	bl sub_8034CB0
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _08022BE0
	adds r0, r6, #0
	bl sub_80231E4
	b _0802268C
	.align 2, 0
_08022BDC: .4byte gUnknown_03001318
_08022BE0:
	add sp, #0x1c
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

