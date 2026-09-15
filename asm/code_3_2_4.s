.include "asm/macros.inc"

.syntax unified
.arm
@ sub_8008044 is reconstructed (extremely close, but not yet
@ byte-matching) as C in src/graphics/actor_part3.c, guarded by
@ #if NON_MATCHING - this raw version is used only for the real
@ byte-matching build. See docs/matching.md, "Parked, not matched:
@ sub_8008044".
.if NON_MATCHING == 0
	thumb_func_start sub_8008044
sub_8008044: @ 0x08008044
	push {r4, r5, lr}
	mov ip, r0
	adds r0, #0x2c
	ldrb r0, [r0]
	cmp r0, #0
	beq _080080BA
	mov r0, ip
	ldr r4, [r0, #0x34]
	ldr r3, [r0, #0x20]
	mov r1, ip
	adds r1, #0x2d
	ldr r2, [r3]
	ldrb r5, [r1]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r2
	adds r2, r1, #0
	ldrb r0, [r0, #0x15]
	cmp r4, r0
	bge _08008076
	adds r0, r4, #1
	mov r1, ip
	str r0, [r1, #0x34]
	b _08008082
_08008076:
	movs r0, #0
	mov r4, ip
	str r0, [r4, #0x34]
	ldr r0, [r4, #0x30]
	adds r0, #1
	str r0, [r4, #0x30]
_08008082:
	mov r5, ip
	ldr r1, [r5, #0x30]
	ldr r3, [r3]
	ldrb r4, [r2]
	lsls r0, r4, #3
	subs r0, r0, r4
	lsls r0, r0, #2
	adds r0, r0, r3
	ldrb r0, [r0, #0x16]
	cmp r1, r0
	blt _080080BA
	movs r0, #0
	str r0, [r5, #0x30]
	str r0, [r5, #0x34]
	ldrb r5, [r2]
	lsls r0, r5, #3
	subs r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r3
	movs r1, #2
	ldrb r0, [r0, #0x17]
	ands r1, r0
	cmp r1, #0
	bne _080080BA
	movs r1, #1
	mov r0, ip
	adds r0, #0x38
	strb r1, [r0]
_080080BA:
	pop {r4, r5}
	pop {r0}
	bx r0

.endif

