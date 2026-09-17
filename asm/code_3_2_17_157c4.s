.include "asm/macros.inc"

.syntax unified
.arm

@ sub_80157C4 is reconstructed (but not yet byte-matching) as C in
@ src/graphics/actor_part28.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-18-0x08014f8c-actor.md, "Parked, not matched:
@ sub_80157C4".
.if NON_MATCHING == 0
	thumb_func_start sub_80157C4
sub_80157C4: @ 0x080157C4
	push {r4, r5, r6, r7, lr}
	adds r6, r0, #0
	adds r7, r1, #0
	adds r5, r2, #0
	ldr r0, _080157EC @ =gUnknown_030012D8
	ldr r1, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	adds r0, r1, r2
	ldrb r0, [r0]
	cmp r0, #0
	beq _0801582A
	cmp r5, #0x12
	beq _080157F6
	cmp r5, #0x12
	bgt _080157F0
	cmp r5, #0xd
	beq _08015800
	b _08015820
	.align 2, 0
_080157EC: .4byte gUnknown_030012D8
_080157F0:
	cmp r5, #0x18
	beq _08015800
	b _08015820
_080157F6:
	ldr r0, [r1, #0x60]
	cmp r0, #0
	beq _0801582A
	movs r5, #0x25
	b _08015802
_08015800:
	movs r5, #0x26
_08015802:
	ldr r4, _0801581C @ =gUnknown_030012BC
	ldr r0, [r4]
	movs r1, #0x36
	bl sub_80019A8
	ldr r0, [r4]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x36
	bl PlaySfx
	b _0801582A
	.align 2, 0
_0801581C: .4byte gUnknown_030012BC
_08015820:
	ldr r0, _0801583C @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r1, #0x36
	bl sub_80019A8
_0801582A:
	adds r0, r6, #0
	adds r1, r7, #0
	adds r2, r5, #0
	bl sub_800B86C
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0801583C: .4byte gUnknown_030012BC

.endif
