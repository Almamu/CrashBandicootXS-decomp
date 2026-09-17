.include "asm/macros.inc"

.syntax unified
.arm
@ sub_8008188 is reconstructed (extremely close, but not yet
@ byte-matching) as C in src/graphics/actor_part4.c, guarded by
@ #if NON_MATCHING - this raw version is used only for the real
@ byte-matching build. See docs/matching.md, "Parked, not matched:
@ sub_8008188".
.if NON_MATCHING == 0
	thumb_func_start sub_8008188
sub_8008188: @ 0x08008188
	adds r3, r0, #0
	subs r0, r1, #1
	cmp r0, #0xb
	bhi _080081FE
	lsls r0, r0, #2
	ldr r1, _0800819C @ =_080081A0
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800819C: .4byte _080081A0
_080081A0: @ jump table
	.4byte _080081DC @ case 0
	.4byte _080081D0 @ case 1
	.4byte _080081FE @ case 2
	.4byte _080081E8 @ case 3
	.4byte _080081FE @ case 4
	.4byte _080081FE @ case 5
	.4byte _080081FE @ case 6
	.4byte _080081EE @ case 7
	.4byte _080081FE @ case 8
	.4byte _080081FE @ case 9
	.4byte _080081FE @ case 10
	.4byte _080081EE @ case 11
_080081D0:
	ldrb r2, [r2, #4]
	lsls r1, r2, #7
	ldr r0, [r3]
	adds r0, r0, r1
	str r0, [r3]
	b _080081FE
_080081DC:
	ldrb r2, [r2, #4]
	lsls r1, r2, #7
	ldr r0, [r3]
	subs r0, r0, r1
	str r0, [r3]
	b _080081FE
_080081E8:
	movs r0, #2
	ldrsh r1, [r2, r0]
	b _080081F6
_080081EE:
	movs r0, #2
	ldrsh r1, [r2, r0]
	ldrb r2, [r2, #5]
	adds r1, r2, r1
_080081F6:
	lsls r1, r1, #8
	ldr r0, [r3, #4]
	subs r0, r0, r1
	str r0, [r3, #4]
_080081FE:
	bx lr

.endif

@ sub_8008200 is reconstructed (extremely close, but not yet
@ byte-matching) as C in src/graphics/actor_part4.c, guarded by
@ #if NON_MATCHING - this raw version is used only for the real
@ byte-matching build. See docs/matching.md, "Parked, not matched:
@ sub_8008200".
.if NON_MATCHING == 0
	thumb_func_start sub_8008200
sub_8008200: @ 0x08008200
	adds r3, r0, #0
	subs r0, r1, #1
	cmp r0, #0xb
	bhi _08008276
	lsls r0, r0, #2
	ldr r1, _08008214 @ =_08008218
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08008214: .4byte _08008218
_08008218: @ jump table
	.4byte _08008254 @ case 0
	.4byte _08008248 @ case 1
	.4byte _08008276 @ case 2
	.4byte _08008260 @ case 3
	.4byte _08008276 @ case 4
	.4byte _08008276 @ case 5
	.4byte _08008276 @ case 6
	.4byte _08008266 @ case 7
	.4byte _08008276 @ case 8
	.4byte _08008276 @ case 9
	.4byte _08008276 @ case 10
	.4byte _08008266 @ case 11
_08008248:
	ldrb r2, [r2, #4]
	lsls r1, r2, #7
	ldr r0, [r3]
	subs r0, r0, r1
	str r0, [r3]
	b _08008276
_08008254:
	ldrb r2, [r2, #4]
	lsls r1, r2, #7
	ldr r0, [r3]
	adds r0, r0, r1
	str r0, [r3]
	b _08008276
_08008260:
	movs r0, #2
	ldrsh r1, [r2, r0]
	b _0800826E
_08008266:
	movs r0, #2
	ldrsh r1, [r2, r0]
	ldrb r2, [r2, #5]
	adds r1, r2, r1
_0800826E:
	lsls r1, r1, #8
	ldr r0, [r3, #4]
	adds r0, r0, r1
	str r0, [r3, #4]
_08008276:
	bx lr

.endif

@ sub_8008278 is reconstructed (extremely close, but not yet
@ byte-matching) as C in src/graphics/actor_part4.c, guarded by
@ #if NON_MATCHING - this raw version is used only for the real
@ byte-matching build. See docs/matching.md, "Parked, not matched:
@ sub_8008278".
.if NON_MATCHING == 0
	thumb_func_start sub_8008278
sub_8008278: @ 0x08008278
	adds r3, r0, #0
	subs r0, r1, #1
	cmp r0, #0xb
	bhi _08008302
	lsls r0, r0, #2
	ldr r1, _0800828C @ =_08008290
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800828C: .4byte _08008290
_08008290: @ jump table
	.4byte _080082CA @ case 0
	.4byte _080082C0 @ case 1
	.4byte _08008302 @ case 2
	.4byte _080082E2 @ case 3
	.4byte _08008302 @ case 4
	.4byte _08008302 @ case 5
	.4byte _08008302 @ case 6
	.4byte _080082E8 @ case 7
	.4byte _08008302 @ case 8
	.4byte _08008302 @ case 9
	.4byte _08008302 @ case 10
	.4byte _080082E8 @ case 11
_080082C0:
	ldrb r0, [r2, #4]
	lsls r1, r0, #7
	ldr r0, [r3]
	subs r0, r0, r1
	b _080082D2
_080082CA:
	ldrb r0, [r2, #4]
	lsls r1, r0, #7
	ldr r0, [r3]
	adds r0, r0, r1
_080082D2:
	str r0, [r3]
	movs r0, #2
	ldrsh r1, [r2, r0]
	lsls r1, r1, #8
	ldr r0, [r3, #4]
	adds r0, r0, r1
	str r0, [r3, #4]
	b _08008302
_080082E2:
	movs r0, #2
	ldrsh r1, [r2, r0]
	b _080082F0
_080082E8:
	movs r0, #2
	ldrsh r1, [r2, r0]
	ldrb r0, [r2, #5]
	adds r1, r0, r1
_080082F0:
	lsls r1, r1, #8
	ldr r0, [r3, #4]
	adds r0, r0, r1
	str r0, [r3, #4]
	ldrb r2, [r2, #4]
	lsls r1, r2, #7
	ldr r0, [r3]
	subs r0, r0, r1
	str r0, [r3]
_08008302:
	bx lr

.endif

