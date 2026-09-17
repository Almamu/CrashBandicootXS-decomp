.include "asm/macros.inc"

.syntax unified
.arm

@ sub_80236EC is reconstructed (but not yet byte-matching) as C in
@ src/system/game_loop6.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-37-game-loop-234e8.md.
.if NON_MATCHING == 0
	thumb_func_start sub_80236EC
sub_80236EC: @ 0x080236EC
	push {r4, r5, lr}
	adds r3, r0, #0
	ldr r2, [r3, #0x74]
	movs r1, #0xa6
	lsls r1, r1, #1
	adds r0, r3, r1
	movs r1, #0x7f
	ands r2, r1
	movs r1, #0x80
	rsbs r1, r1, #0
	ldrb r4, [r0]
	ands r1, r4
	orrs r1, r2
	strb r1, [r0]
	ldr r2, [r3, #0x6c]
	ldr r5, _08023730 @ =0x0000014D
	lsls r2, r2, #1
	movs r1, #1
	ldrb r4, [r5, r3]
	ands r1, r4
	orrs r1, r2
	strb r1, [r5, r3]
	ldr r2, [r3, #0x78]
	movs r1, #3
	ands r2, r1
	lsls r2, r2, #7
	ldr r1, _08023734 @ =0xFFFFFE7F
	ldrh r5, [r0]
	ands r1, r5
	orrs r1, r2
	strh r1, [r0]
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08023730: .4byte 0x0000014D
_08023734: .4byte 0xFFFFFE7F


.endif
