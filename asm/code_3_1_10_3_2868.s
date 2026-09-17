.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8002868/sub_8002938 are reconstructed (semantics fully understood)
@ but NOT YET byte-matching - parked as C in
@ src/graphics/settings_menu8d.c, guarded by #if NON_MATCHING. See
@ docs/matching/issue-4-sio-settings-sync.md.
.if NON_MATCHING == 0
	thumb_func_start sub_8002868
sub_8002868: @ 0x08002868
	push {r4, r5, r6, r7, lr}
	ldr r4, _080028A0 @ =0xFFFFFE00
	add sp, r4
	adds r6, r0, #0
	adds r7, r1, #0
	ldr r4, _080028A4 @ =gUnknown_03000808
	ldrb r0, [r4]
	cmp r0, #0
	beq _0800288A
	movs r0, #4
	bl sub_803A968
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	cmp r0, #0
	bne _0800291C
	strb r0, [r4]
_0800288A:
	ldr r1, _080028A8 @ =0x04000208
	movs r0, #0
	strh r0, [r1]
	ldr r1, _080028AC @ =gUnknown_030009FC
	movs r0, #2
	bl sub_803A9D0
	mov r5, sp
	movs r4, #0
	b _080028C4
	.align 2, 0
_080028A0: .4byte 0xFFFFFE00
_080028A4: .4byte gUnknown_03000808
_080028A8: .4byte 0x04000208
_080028AC: .4byte gUnknown_030009FC
_080028B0:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	adds r1, r5, #0
	bl sub_803AB54
	lsls r0, r0, #0x10
	cmp r0, #0
	bne _08002904
	adds r5, #8
	adds r4, #1
_080028C4:
	ldr r0, _080028F4 @ =gUnknown_03001634
	ldr r0, [r0]
	ldrh r0, [r0, #4]
	cmp r4, r0
	blt _080028B0
	ldr r3, _080028F8 @ =0x04000208
	ldrh r2, [r3]
	movs r0, #0
	strh r0, [r3]
	ldr r4, _080028FC @ =0x04000200
	ldrh r1, [r4]
	ldr r0, _08002900 @ =0x0000FFDF
	ands r0, r1
	strh r0, [r4]
	strh r2, [r3]
	movs r0, #1
	strh r0, [r3]
	adds r0, r6, #0
	mov r1, sp
	adds r2, r7, #0
	bl sub_800014C
	movs r0, #0
	b _08002920
	.align 2, 0
_080028F4: .4byte gUnknown_03001634
_080028F8: .4byte 0x04000208
_080028FC: .4byte 0x04000200
_08002900: .4byte 0x0000FFDF
_08002904:
	ldr r3, _0800292C @ =0x04000208
	ldrh r2, [r3]
	movs r0, #0
	strh r0, [r3]
	ldr r4, _08002930 @ =0x04000200
	ldrh r1, [r4]
	ldr r0, _08002934 @ =0x0000FFDF
	ands r0, r1
	strh r0, [r4]
	strh r2, [r3]
	movs r0, #1
	strh r0, [r3]
_0800291C:
	movs r0, #1
	rsbs r0, r0, #0
_08002920:
	movs r3, #0x80
	lsls r3, r3, #2
	add sp, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0800292C: .4byte 0x04000208
_08002930: .4byte 0x04000200
_08002934: .4byte 0x0000FFDF

	thumb_func_start sub_8002938
sub_8002938: @ 0x08002938
	push {r4, r5, r6, lr}
	ldr r4, _08002978 @ =0xFFFFFE00
	add sp, r4
	adds r4, r0, #0
	adds r5, r1, #0
	ldr r6, _0800297C @ =gUnknown_03000808
	ldrb r0, [r6]
	cmp r0, #0
	beq _0800295A
	movs r0, #4
	bl sub_803A968
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	cmp r0, #0
	bne _080029EC
	strb r0, [r6]
_0800295A:
	mov r0, sp
	adds r1, r4, #0
	adds r2, r5, #0
	bl sub_800014C
	ldr r1, _08002980 @ =0x04000208
	movs r0, #0
	strh r0, [r1]
	ldr r1, _08002984 @ =gUnknown_030009FC
	movs r0, #2
	bl sub_803A9D0
	mov r5, sp
	movs r4, #0
	b _0800299C
	.align 2, 0
_08002978: .4byte 0xFFFFFE00
_0800297C: .4byte gUnknown_03000808
_08002980: .4byte 0x04000208
_08002984: .4byte gUnknown_030009FC
_08002988:
	lsls r0, r4, #0x10
	lsrs r0, r0, #0x10
	adds r1, r5, #0
	bl sub_803AD38
	lsls r0, r0, #0x10
	cmp r0, #0
	bne _080029D4
	adds r5, #8
	adds r4, #1
_0800299C:
	ldr r0, _080029C4 @ =gUnknown_03001634
	ldr r0, [r0]
	ldrh r0, [r0, #4]
	cmp r4, r0
	blt _08002988
	ldr r3, _080029C8 @ =0x04000208
	ldrh r2, [r3]
	movs r0, #0
	strh r0, [r3]
	ldr r4, _080029CC @ =0x04000200
	ldrh r1, [r4]
	ldr r0, _080029D0 @ =0x0000FFDF
	ands r0, r1
	strh r0, [r4]
	strh r2, [r3]
	movs r0, #1
	strh r0, [r3]
	movs r0, #0
	b _080029F0
	.align 2, 0
_080029C4: .4byte gUnknown_03001634
_080029C8: .4byte 0x04000208
_080029CC: .4byte 0x04000200
_080029D0: .4byte 0x0000FFDF
_080029D4:
	ldr r3, _080029FC @ =0x04000208
	ldrh r2, [r3]
	movs r0, #0
	strh r0, [r3]
	ldr r4, _08002A00 @ =0x04000200
	ldrh r1, [r4]
	ldr r0, _08002A04 @ =0x0000FFDF
	ands r0, r1
	strh r0, [r4]
	strh r2, [r3]
	movs r0, #1
	strh r0, [r3]
_080029EC:
	movs r0, #1
	rsbs r0, r0, #0
_080029F0:
	movs r3, #0x80
	lsls r3, r3, #2
	add sp, r3
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_080029FC: .4byte 0x04000208
_08002A00: .4byte 0x04000200
_08002A04: .4byte 0x0000FFDF
.endif
