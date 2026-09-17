.include "asm/macros.inc"

.syntax unified
.arm

@ sub_802AB58 is reconstructed (but not yet byte-matching) as C in
@ src/graphics/actor_part42.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-50-actor-2a69c.md.
.if NON_MATCHING == 0
	thumb_func_start sub_802AB58
sub_802AB58: @ 0x0802AB58
	push {r4, lr}
	ldr r0, _0802ABA8 @ =gUnknown_03001464
	ldrb r0, [r0]
	cmp r0, #0
	beq _0802ABC0
	ldr r4, _0802ABAC @ =gUnknown_03001470
	ldr r1, [r4]
	lsls r0, r1, #3
	subs r0, r0, r1
	lsls r0, r0, #6
	ldr r1, _0802ABB0 @ =gStaticData_08175760
	adds r0, r0, r1
	movs r1, #0xa0
	lsls r1, r1, #0x13
	movs r2, #0xe0
	lsls r2, r2, #1
	movs r3, #0x10
	bl QueueVramDmaTransfer
	ldr r1, _0802ABB4 @ =gUnknown_03001478
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
	cmp r0, #0x23
	ble _0802ABC0
	movs r0, #0
	str r0, [r1]
	adds r1, r4, #0
	ldr r0, _0802ABB8 @ =gUnknown_03001474
	ldr r3, [r0]
	ldr r2, [r1]
	subs r0, r3, r2
	cmp r0, #0
	blt _0802ABBC
	adds r0, r2, #0
	cmp r3, r0
	beq _0802ABBE
	adds r0, #1
	b _0802ABBE
	.align 2, 0
_0802ABA8: .4byte gUnknown_03001464
_0802ABAC: .4byte gUnknown_03001470
_0802ABB0: .4byte gStaticData_08175760
_0802ABB4: .4byte gUnknown_03001478
_0802ABB8: .4byte gUnknown_03001474
_0802ABBC:
	subs r0, r2, #1
_0802ABBE:
	str r0, [r1]
_0802ABC0:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
.endif
