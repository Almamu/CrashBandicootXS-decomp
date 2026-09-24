.include "asm/macros.inc"

.syntax unified
.arm

@ sub_80250BC/sub_8025130/sub_8025228/sub_8025334 are
@ reconstructed (but not yet byte-matching) as C in
@ src/system/game_loop3.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build.
@ sub_8024F24 (formerly the first function in this block) is now
@ matched as a NAKED transcription in src/system/game_loop3.c and no
@ longer lives here. See docs/matching/issue-40-terrain-tile-cache.md.
.if NON_MATCHING == 0
	thumb_func_start sub_80250BC
sub_80250BC: @ 0x080250BC
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	adds r5, r0, #0
	adds r4, r1, #0
	adds r6, r2, #0
	mov r7, sp
	cmp r4, #0
	blt _08025116
	cmp r6, #0
	blt _08025116
	asrs r3, r4, #4
	asrs r1, r6, #3
	ldr r2, [r5]
	ldr r0, [r5, #0x18]
	muls r0, r1, r0
	adds r0, r0, r3
	ldr r1, [r2]
	lsls r0, r0, #1
	adds r0, r0, r1
	ldrh r1, [r0]
	adds r0, r5, #0
	bl sub_8024F24
	movs r1, #7
	ands r1, r6
	movs r2, #0xf
	ands r4, r2
	lsls r1, r1, #4
	adds r1, r1, r4
	lsls r1, r1, #1
	adds r1, r1, r0
	ldrh r1, [r1]
	lsls r0, r1, #0x10
	lsrs r3, r0, #0x10
	lsrs r0, r0, #0x18
	ands r0, r2
	cmp r0, #0
	beq _0802510A
	strb r0, [r7]
_0802510A:
	movs r1, #0xff
	ands r1, r3
	cmp r1, #0
	beq _08025116
	cmp r1, #0x23
	ble _0802511A
_08025116:
	movs r0, #0
	b _08025124
_0802511A:
	lsls r0, r1, #3
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _0802512C @ =gStaticData_081725AC
	adds r0, r0, r1
_08025124:
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0802512C: .4byte gStaticData_081725AC

	thumb_func_start sub_8025130
sub_8025130: @ 0x08025130
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	adds r4, r1, #0
	adds r6, r2, #0
	adds r7, r3, #0
	movs r0, #0
	mov r8, r0
	movs r3, #0
	cmp r4, #0
	blt _0802514C
	cmp r6, #0
	bge _08025150
_0802514C:
	movs r1, #0
	b _08025192
_08025150:
	asrs r3, r4, #4
	asrs r1, r6, #3
	ldr r2, [r5]
	ldr r0, [r5, #0x18]
	muls r0, r1, r0
	adds r0, r0, r3
	ldr r1, [r2]
	lsls r0, r0, #1
	adds r0, r0, r1
	ldrh r1, [r0]
	adds r0, r5, #0
	bl sub_8024F24
	movs r1, #7
	ands r1, r6
	movs r2, #0xf
	ands r4, r2
	lsls r1, r1, #4
	adds r1, r1, r4
	lsls r1, r1, #1
	adds r1, r1, r0
	ldrh r1, [r1]
	lsls r0, r1, #0x10
	lsrs r4, r0, #0x10
	lsrs r3, r0, #0x1c
	lsrs r1, r0, #0x18
	ands r1, r2
	cmp r1, #0
	beq _0802518E
	ldr r0, [sp, #0x18]
	strb r1, [r0]
_0802518E:
	movs r1, #0xff
	ands r1, r4
_08025192:
	cmp r1, #0x23
	bgt _0802519A
	movs r0, #0
	b _0802521A
_0802519A:
	cmp r7, #1
	beq _080251CC
	cmp r7, #1
	bgt _080251A8
	cmp r7, #0
	beq _080251B2
	b _08025218
_080251A8:
	cmp r7, #2
	beq _080251E4
	cmp r7, #3
	beq _08025200
	b _08025218
_080251B2:
	movs r0, #4
	ands r3, r0
	cmp r3, #0
	beq _080251BE
	movs r0, #0
	b _08025216
_080251BE:
	lsls r0, r1, #3
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _080251C8 @ =gStaticData_081725AC
	b _08025214
	.align 2, 0
_080251C8: .4byte gStaticData_081725AC
_080251CC:
	ands r3, r7
	cmp r3, #0
	beq _080251D6
	movs r0, #0
	b _08025216
_080251D6:
	lsls r0, r1, #3
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _080251E0 @ =gStaticData_081725B4
	b _08025214
	.align 2, 0
_080251E0: .4byte gStaticData_081725B4
_080251E4:
	movs r0, #8
	ands r3, r0
	cmp r3, #0
	beq _080251F0
	movs r0, #0
	b _08025216
_080251F0:
	lsls r0, r1, #3
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _080251FC @ =gStaticData_081725BC
	b _08025214
	.align 2, 0
_080251FC: .4byte gStaticData_081725BC
_08025200:
	movs r0, #2
	ands r3, r0
	cmp r3, #0
	beq _0802520C
	movs r0, #0
	b _08025216
_0802520C:
	lsls r0, r1, #3
	adds r0, r0, r1
	lsls r0, r0, #2
	ldr r1, _08025224 @ =gStaticData_081725C4
_08025214:
	adds r0, r0, r1
_08025216:
	mov r8, r0
_08025218:
	mov r0, r8
_0802521A:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08025224: .4byte gStaticData_081725C4

	thumb_func_start sub_8025228
sub_8025228: @ 0x08025228
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	adds r4, r1, #0
	adds r6, r2, #0
	adds r7, r3, #0
	movs r0, #0
	mov r8, r0
	movs r3, #0
	cmp r4, #0
	blt _08025244
	cmp r6, #0
	bge _08025248
_08025244:
	movs r2, #0
	b _0802528A
_08025248:
	asrs r3, r4, #4
	asrs r1, r6, #3
	ldr r2, [r5]
	ldr r0, [r5, #0x18]
	muls r0, r1, r0
	adds r0, r0, r3
	ldr r1, [r2]
	lsls r0, r0, #1
	adds r0, r0, r1
	ldrh r1, [r0]
	adds r0, r5, #0
	bl sub_8024F24
	movs r1, #7
	ands r1, r6
	movs r2, #0xf
	ands r4, r2
	lsls r1, r1, #4
	adds r1, r1, r4
	lsls r1, r1, #1
	adds r1, r1, r0
	ldrh r1, [r1]
	lsls r0, r1, #0x10
	lsrs r4, r0, #0x10
	lsrs r3, r0, #0x1c
	lsrs r1, r0, #0x18
	ands r1, r2
	cmp r1, #0
	beq _08025286
	ldr r0, [sp, #0x18]
	strb r1, [r0]
_08025286:
	movs r2, #0xff
	ands r2, r4
_0802528A:
	cmp r2, #0x23
	bgt _08025294
	movs r0, #1
	rsbs r0, r0, #0
	b _08025324
_08025294:
	cmp r7, #1
	beq _080252C8
	cmp r7, #1
	bgt _080252A2
	cmp r7, #0
	beq _080252AC
	b _0802531E
_080252A2:
	cmp r7, #2
	beq _080252E4
	cmp r7, #3
	beq _08025304
	b _0802531E
_080252AC:
	movs r0, #4
	ands r3, r0
	cmp r3, #0
	bne _080252EC
	ldr r1, _080252C4 @ =gStaticData_081725A8
	lsls r0, r2, #3
	adds r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0]
	b _0802531C
	.align 2, 0
_080252C4: .4byte gStaticData_081725A8
_080252C8:
	ands r3, r7
	cmp r3, #0
	beq _080252D2
	movs r0, #0
	b _0802531C
_080252D2:
	ldr r1, _080252E0 @ =gStaticData_081725A8
	lsls r0, r2, #3
	adds r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #1]
	b _0802531C
	.align 2, 0
_080252E0: .4byte gStaticData_081725A8
_080252E4:
	movs r0, #8
	ands r3, r0
	cmp r3, #0
	beq _080252F2
_080252EC:
	movs r1, #0
	mov r8, r1
	b _0802531E
_080252F2:
	ldr r1, _08025300 @ =gStaticData_081725A8
	lsls r0, r2, #3
	adds r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #2]
	b _0802531C
	.align 2, 0
_08025300: .4byte gStaticData_081725A8
_08025304:
	movs r0, #2
	ands r3, r0
	cmp r3, #0
	beq _08025310
	movs r0, #0
	b _0802531C
_08025310:
	ldr r1, _08025330 @ =gStaticData_081725A8
	lsls r0, r2, #3
	adds r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r0, #3]
_0802531C:
	mov r8, r0
_0802531E:
	mov r1, r8
	lsls r0, r1, #0x18
	asrs r0, r0, #0x18
_08025324:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08025330: .4byte gStaticData_081725A8

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
