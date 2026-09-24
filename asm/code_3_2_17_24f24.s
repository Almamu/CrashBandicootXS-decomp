.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8025334 is reconstructed (but not yet byte-matching) as C in
@ src/system/game_loop3.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build.
@ sub_8024F24/sub_80250BC/sub_8025130/sub_8025228 (formerly the first
@ four functions in this block) are now matched as NAKED transcriptions
@ in src/system/game_loop3.c and no longer live here. See
@ docs/matching/issue-40-terrain-tile-cache.md.
.if NON_MATCHING == 0
	thumb_func_start sub_8025334
sub_8025334: @ 0x08025334
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	mov r8, r2
	ldr r6, [r0, #4]
	lsls r1, r1, #1
	adds r1, r1, r6
	ldrh r1, [r1]
	lsls r0, r1, #2
	adds r6, r6, r0
	movs r0, #0x7f
	mov sb, r0
	movs r1, #0
	mov ip, r1
	mov r7, r8
_08025354:
	ldrh r1, [r6]
	ldrb r3, [r6]
	adds r6, #2
	movs r0, #0x80
	lsls r0, r0, #8
	ands r0, r1
	cmp r0, #0
	beq _0802538C
	ldrh r2, [r6]
	adds r6, #2
	mov r4, sb
	subs r4, r4, r3
	mov sb, r4
	mov r1, ip
	lsls r0, r1, #1
	mov r4, r8
	adds r1, r0, r4
_08025376:
	strh r2, [r1]
	adds r1, #2
	adds r7, #2
	movs r0, #1
	add ip, r0
	subs r0, r3, #1
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	cmp r3, #0
	bne _08025376
	b _08025430
_0802538C:
	movs r0, #0x80
	lsls r0, r0, #7
	ands r1, r0
	cmp r1, #0
	beq _0802540A
	mov r1, sb
	subs r1, r1, r3
	mov sb, r1
	ldrh r4, [r6]
	adds r6, #2
	strh r4, [r7]
	subs r0, r3, #1
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	adds r7, #2
	movs r2, #1
	add ip, r2
	mov r1, ip
	lsls r0, r1, #1
	mov r2, r8
	adds r5, r0, r2
_080253B6:
	ldrh r2, [r6]
	adds r6, #2
	lsls r1, r2, #0x18
	lsls r0, r4, #0x10
	asrs r0, r0, #0x10
	asrs r1, r1, #0x18
	adds r0, r0, r1
	lsls r0, r0, #0x10
	lsrs r4, r0, #0x10
	strh r4, [r5]
	adds r5, #2
	lsls r2, r2, #0x10
	lsls r0, r4, #0x10
	asrs r0, r0, #0x10
	asrs r2, r2, #0x18
	adds r0, r0, r2
	lsls r0, r0, #0x10
	lsrs r4, r0, #0x10
	strh r4, [r5]
	adds r5, #2
	adds r7, #4
	movs r0, #2
	add ip, r0
	subs r0, r3, #2
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	cmp r3, #1
	bhi _080253B6
	cmp r3, #0
	beq _08025430
	ldrh r1, [r6]
	adds r6, #2
	lsls r1, r1, #0x18
	lsls r0, r4, #0x10
	asrs r0, r0, #0x10
	asrs r1, r1, #0x18
	adds r0, r0, r1
	strh r0, [r7]
	adds r7, #2
	movs r1, #1
	add ip, r1
	b _08025430
_0802540A:
	mov r2, sb
	subs r2, r2, r3
	mov sb, r2
	mov r4, ip
	lsls r0, r4, #1
	mov r2, r8
	adds r1, r0, r2
_08025418:
	ldrh r0, [r6]
	strh r0, [r1]
	adds r6, #2
	adds r1, #2
	adds r7, #2
	movs r4, #1
	add ip, r4
	subs r0, r3, #1
	lsls r0, r0, #0x10
	lsrs r3, r0, #0x10
	cmp r3, #0
	bne _08025418
_08025430:
	mov r0, sb
	cmp r0, #0
	bge _08025354
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0


.endif
