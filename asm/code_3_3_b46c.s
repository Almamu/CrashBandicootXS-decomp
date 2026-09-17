.include "asm/macros.inc"

.syntax unified
.arm

@ sub_803B46C is reconstructed (but not yet byte-matching) as C in
@ src/graphics/actor_anim.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-71-0x0803b060-actor.md, "Parked, not matched:
@ sub_803B46C".
.if NON_MATCHING == 0
	thumb_func_start sub_803B46C
sub_803B46C: @ 0x0803B46C
	push	{r4, r5, r6, r7, lr}
	adds	r5, r0, #0
	movs	r4, #120	@ 0x78
	movs	r6, #106	@ 0x6a
	bl GetAnimFrameData
	adds	r7, r0, #0
	ldrb	r0, [r7, #0]
	lsls	r2, r0, #2
	ldrb	r1, [r7, #1]
	lsls	r0, r1, #2
	subs	r4, r4, r2
	subs	r6, r6, r0
	cmp	r6, #159	@ 0x9f
	bgt _0803B4E4
	lsls	r0, r1, #3
	adds	r0, r6, r0
	cmp	r0, #0
	blt _0803B4E4
	cmp	r4, #239	@ 0xef
	bgt _0803B4E4
	lsls	r0, r2, #1
	adds	r0, r4, r0
	cmp	r0, #0
	blt _0803B4E4
	adds	r0, r5, #0
	bl sub_803B060
	movs	r3, #255	@ 0xff
	ands	r3, r6
	ldr r1, _0803B4D0
	ands	r4, r1
	lsls	r1, r4, #16
	orrs	r3, r1
	orrs	r3, r0
	movs	r0, #0
	orrs	r3, r0
	ldr	r4, [r5, #24]
	lsls	r2, r4, #12
	ldr	r0, [r5, #20]
	movs	r1, #128	@ 0x80
	lsls	r1, r1, #8
_0803B4C0: .4byte 0x28004008
	beq _0803B4D4
	movs	r0, #128	@ 0x80
	lsls	r0, r0, #4
	orrs	r2, r0
	lsls	r0, r2, #16
	b _0803B4D6
_0803B4D0: .4byte 0x1ff
_0803B4D4:
	lsls	r0, r4, #28
_0803B4D6:
	lsrs	r2, r0, #16
	adds	r0, r7, #0
	adds	r1, r3, #0
	movs	r3, #128	@ 0x80
	lsls	r3, r3, #1
	bl SetupSpriteFrameOam
_0803B4E4:
	pop	{r4, r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.align 2, 0

.endif
