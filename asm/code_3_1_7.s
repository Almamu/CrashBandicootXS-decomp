.include "asm/macros.inc"

.syntax unified
.arm

@ sub_80014A4 is reconstructed (semantics fully understood, but not
@ yet byte-matching) as C in src/graphics/fade_screen_mode.c, guarded
@ by #if NON_MATCHING - see docs/matching.md for the exact remaining
@ gap.
.if NON_MATCHING == 0
	thumb_func_start sub_80014A4
sub_80014A4: @ 0x080014A4
	push {r4, r5, r6, lr}
	ldr r1, _080014FC @ =0x040000D4
	movs r0, #0xa0
	lsls r0, r0, #0x13
	str r0, [r1]
	ldr r0, _08001500 @ =gUnknown_03000A80
	str r0, [r1, #4]
	ldr r0, _08001504 @ =0x80000200
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	movs r5, #0
	adds r4, r1, #0
	ldr r6, _08001508 @ =gUnknown_03000E80
_080014BE:
	adds r0, r5, #0
	bl sub_80013FC
	bl sub_80006A8
	str r6, [r4]
	movs r3, #0xa0
	lsls r3, r3, #0x13
	str r3, [r4, #4]
	ldr r2, _08001504 @ =0x80000200
	str r2, [r4, #8]
	ldr r0, [r4, #8]
	adds r5, #2
	cmp r5, #0x10
	ble _080014BE
	ldr r1, _0800150C @ =0x04000050
	movs r0, #0xff
	strh r0, [r1]
	adds r1, #4
	movs r0, #0x10
	strh r0, [r1]
	ldr r0, _080014FC @ =0x040000D4
	ldr r1, _08001500 @ =gUnknown_03000A80
	str r1, [r0]
	str r3, [r0, #4]
	str r2, [r0, #8]
	ldr r0, [r0, #8]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_080014FC: .4byte 0x040000D4
_08001500: .4byte gUnknown_03000A80
_08001504: .4byte 0x80000200
_08001508: .4byte gUnknown_03000E80
_0800150C: .4byte 0x04000050
.endif
