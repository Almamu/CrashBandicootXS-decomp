.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8024708 is reconstructed (but not yet byte-matching) as C in
@ src/system/game_loop35.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-38-sound-channel-family.md.
.if NON_MATCHING == 0
	thumb_func_start sub_8024708
sub_8024708: @ 0x08024708
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	ldr r0, [r5]
	lsls r1, r1, #2
	adds r1, r1, r0
	ldr r0, [r1]
	ldr r6, [r0]
	ldr r0, [r5, #0xc]
	movs r1, #1
	eors r0, r1
	str r0, [r5, #0xc]
	cmp r0, #0
	bne _08024732
	movs r1, #0x80
	lsls r1, r1, #2
	adds r0, r6, r1
	movs r1, #0xc0
	lsls r1, r1, #0x13
	bl LoadTaggedAsset
	b _0802473E
_08024732:
	movs r2, #0x80
	lsls r2, r2, #2
	adds r0, r6, r2
	ldr r1, _08024774 @ =0x0600A000
	bl LoadTaggedAsset
_0802473E:
	ldr r4, _08024778 @ =gUnknown_03001314
	movs r1, #1
	ldrb r5, [r5, #0xc]
	ands r1, r5
	lsls r1, r1, #4
	movs r0, #0x11
	rsbs r0, r0, #0
	ldrb r2, [r4]
	ands r0, r2
	orrs r0, r1
	strb r0, [r4]
	bl sub_80006A8
	ldr r1, _0802477C @ =0x040000D4
	str r6, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	str r0, [r1, #4]
	ldr r0, _08024780 @ =0x80000100
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	subs r1, #0xd4
	ldrh r0, [r4]
	strh r0, [r1]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08024774: .4byte 0x0600A000
_08024778: .4byte gUnknown_03001314
_0802477C: .4byte 0x040000D4
_08024780: .4byte 0x80000100
.endif
