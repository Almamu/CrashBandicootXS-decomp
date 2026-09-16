.include "asm/macros.inc"

.syntax unified
.arm
@ sub_80083B8 is reconstructed (extremely close, but not yet
@ byte-matching) as C in src/graphics/actor_part5.c, guarded by
@ #if NON_MATCHING - this raw version is used only for the real
@ byte-matching build. See docs/matching.md, "Parked, not matched:
@ sub_80083B8".
.if NON_MATCHING == 0
	thumb_func_start sub_80083B8
sub_80083B8: @ 0x080083B8
	push {r4, lr}
	adds r3, r0, #0
	ldr r1, [r3, #0x20]
	adds r2, r3, #0
	adds r2, #0x2d
	ldrb r4, [r2]
	lsls r0, r4, #3
	subs r0, r0, r4
	lsls r0, r0, #2
	ldr r1, [r1]
	adds r1, r1, r0
	adds r0, r3, #0
	adds r0, #0x38
	ldrb r0, [r0]
	cmp r0, #0
	beq _080083EC
	movs r0, #2
	ldrb r2, [r1, #0x17]
	ands r0, r2
	cmp r0, #0
	bne _080083EC
	ldrb r0, [r1, #0x16]
	subs r0, #1
	str r0, [r3, #0x30]
	ldrb r0, [r1, #0x15]
	str r0, [r3, #0x34]
_080083EC:
	ldr r2, [r3, #0x20]
	ldr r0, [r3, #0x30]
	ldr r1, [r1]
	lsls r0, r0, #1
	adds r0, r0, r1
	ldr r1, [r2, #4]
	ldrh r0, [r0]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0

.endif

