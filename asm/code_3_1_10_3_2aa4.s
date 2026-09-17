.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8002AA4 is reconstructed (semantics fully understood) but NOT YET
@ byte-matching - parked as C in src/graphics/settings_menu8e.c, guarded
@ by #if NON_MATCHING. See docs/matching/issue-4-sio-settings-sync.md.
.if NON_MATCHING == 0
	thumb_func_start sub_8002AA4
sub_8002AA4: @ 0x08002AA4
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	adds r5, r0, #0
	adds r3, r5, #0
	movs r2, #0
	movs r1, #0x7e
_08002AB6:
	ldm r3!, {r0}
	adds r2, r2, r0
	subs r1, #1
	cmp r1, #0
	bge _08002AB6
	movs r1, #0
	movs r3, #0xfe
	lsls r3, r3, #1
	adds r0, r5, r3
	ldr r0, [r0]
	cmp r2, r0
	bne _08002AD0
	movs r1, #1
_08002AD0:
	cmp r1, #0
	bne _08002B26
	mov r0, sp
	strh r1, [r0]
	ldr r0, _08002B34 @ =0x040000D4
	mov r1, sp
	str r1, [r0]
	str r5, [r0, #4]
	ldr r1, _08002B38 @ =0x81000100
	str r1, [r0, #8]
	ldr r0, [r0, #8]
	movs r4, #0
	movs r2, #0xfc
	lsls r2, r2, #1
	adds r6, r5, r2
	ldr r3, _08002B3C @ =0x000001F9
	adds r3, r3, r5
	mov sb, r3
	movs r0, #0xfd
	lsls r0, r0, #1
	adds r7, r5, r0
	ldr r1, _08002B40 @ =0x000001FB
	adds r1, r1, r5
	mov r8, r1
_08002B00:
	adds r0, r5, #0
	adds r1, r4, #0
	bl sub_8002C6C
	adds r4, #1
	cmp r4, #3
	ble _08002B00
	movs r0, #0
	movs r1, #0x43
	strb r1, [r6]
	movs r1, #0x12
	mov r2, sb
	strb r1, [r2]
	strb r0, [r7]
	mov r3, r8
	strb r0, [r3]
	adds r0, r5, #0
	bl sub_8002B70
_08002B26:
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08002B34: .4byte 0x040000D4
_08002B38: .4byte 0x81000100
_08002B3C: .4byte 0x000001F9
_08002B40: .4byte 0x000001FB
.endif
