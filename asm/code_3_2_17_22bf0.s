.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8022BF0/sub_8022CA0 are reconstructed (but not yet byte-matching)
@ as C in src/system/game_loop.c, guarded by #if NON_MATCHING - this
@ raw version is only assembled for the default (matching) build. See
@ docs/matching.md, GitHub issue #34's entry.
.if NON_MATCHING == 0
	thumb_func_start sub_8022BF0
sub_8022BF0: @ 0x08022BF0
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	lsls r4, r1, #0x18
	lsrs r4, r4, #0x18
	bl sub_80232FC
	cmp r4, #0
	beq _08022C68
	adds r1, r5, #0
	adds r1, #0xb4
	ldr r0, [r5, #0x70]
	ldr r1, [r1]
	adds r0, r0, r1
	str r0, [r5, #0x70]
	adds r0, r5, #0
	adds r0, #0xb0
	ldr r1, [r5, #0x6c]
	ldr r0, [r0]
	adds r1, r1, r0
	str r1, [r5, #0x6c]
	adds r0, r5, #0
	adds r0, #0xb8
	adds r4, r5, #0
	adds r4, #0xd4
	adds r7, r5, #0
	adds r7, #0xe0
	adds r6, r5, #0
	adds r6, #0xbc
	cmp r1, #0x63
	ble _08022C3A
	ldr r2, [r5, #0x74]
_08022C2E:
	adds r2, #1
	subs r1, #0x64
	cmp r1, #0x63
	bgt _08022C2E
	str r1, [r5, #0x6c]
	str r2, [r5, #0x74]
_08022C3A:
	ldr r1, [r5, #0x74]
	ldr r0, [r0]
	adds r0, r1, r0
	cmp r0, #0x63
	ble _08022C46
	movs r0, #0x63
_08022C46:
	str r0, [r5, #0x74]
	adds r0, r5, #0
	bl sub_80232B0
	ldr r0, _08022C64 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r1, [r4]
	ldr r2, [r4, #4]
	bl sub_8007398
	ldrb r1, [r7]
	adds r0, r5, #0
	bl sub_8022CA0
	b _08022C84
	.align 2, 0
_08022C64: .4byte gUnknown_030012D8
_08022C68:
	adds r0, r5, #0
	adds r0, #0xb4
	ldr r0, [r0]
	str r0, [r5, #0x70]
	adds r0, r5, #0
	adds r0, #0xb0
	ldr r0, [r0]
	str r0, [r5, #0x6c]
	adds r0, r5, #0
	adds r0, #0xb8
	ldr r0, [r0]
	str r0, [r5, #0x74]
	adds r6, r5, #0
	adds r6, #0xbc
_08022C84:
	ldr r0, _08022C9C @ =gUnknown_03001318
	ldr r0, [r0]
	ldr r1, [r6]
	bl sub_8028568
	adds r0, r5, #0
	bl sub_80232C0
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08022C9C: .4byte gUnknown_03001318

	thumb_func_start sub_8022CA0
sub_8022CA0: @ 0x08022CA0
	push {r4, r5, r6, lr}
	adds r6, r0, #0
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	adds r0, #0xdc
	ldr r0, [r0]
	ldr r0, [r0, #8]
	cmp r0, #3
	bne _08022CD4
	adds r0, r6, #0
	bl sub_8023414
	adds r1, r6, #0
	adds r1, #0xcc
	str r0, [r1]
	adds r0, r6, #0
	adds r0, #0xa9
	ldrb r1, [r0]
	adds r0, #0x27
	strb r1, [r0]
	adds r0, #0x14
	adds r1, r6, #0
	movs r2, #0x68
	bl sub_800014C
	b _08022D3E
_08022CD4:
	ldr r0, _08022D44 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r4, [r0]
	ldr r5, [r0, #4]
	adds r0, r6, #0
	adds r0, #0xe0
	strb r1, [r0]
	adds r0, r6, #0
	bl sub_8023414
	adds r1, r6, #0
	adds r1, #0xcc
	str r0, [r1]
	adds r0, r6, #0
	adds r0, #0xa9
	ldrb r1, [r0]
	adds r0, #0x27
	strb r1, [r0]
	adds r0, r6, #0
	bl sub_80232FC
	adds r0, r6, #0
	bl sub_80232EC
	adds r0, r6, #0
	adds r0, #0xd4
	str r4, [r0]
	str r5, [r0, #4]
	ldr r0, _08022D48 @ =gUnknown_030012B4
	ldr r4, [r0]
	movs r1, #0x84
	lsls r1, r1, #1
	adds r0, r4, r1
	adds r1, r4, #0
	adds r1, #8
	ldr r2, _08022D4C @ =0x04000040
	bl sub_803A94C
	movs r2, #0xc2
	lsls r2, r2, #2
	adds r0, r4, r2
	movs r2, #0x82
	lsls r2, r2, #2
	adds r1, r4, r2
	ldr r2, _08022D4C @ =0x04000040
	bl sub_803A94C
	adds r0, r6, #0
	adds r0, #0xe4
	adds r1, r6, #0
	movs r2, #0x68
	bl sub_800014C
_08022D3E:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08022D44: .4byte gUnknown_030012D8
_08022D48: .4byte gUnknown_030012B4
_08022D4C: .4byte 0x04000040

.endif
