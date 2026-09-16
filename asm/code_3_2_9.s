.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8009DF4 is reconstructed (semantics fully understood, but not
@ yet byte-matching) as C in src/graphics/actor_part8.c, guarded by
@ #if NON_MATCHING - see docs/matching.md for the exact remaining gap.
.if NON_MATCHING == 0
	thumb_func_start sub_8009DF4
sub_8009DF4: @ 0x08009DF4
	adds r2, r0, #0
	ldr r1, [r2, #0x60]
	ldr r3, [r2, #0x50]
	cmp r1, r3
	bge _08009E0A
	ldr r0, [r2, #0x4c]
	adds r0, r1, r0
	str r0, [r2, #0x60]
	cmp r0, r3
	ble _08009E1A
	b _08009E18
_08009E0A:
	cmp r1, r3
	ble _08009E1A
	ldr r0, [r2, #0x4c]
	subs r0, r1, r0
	str r0, [r2, #0x60]
	cmp r0, r3
	bge _08009E1A
_08009E18:
	str r3, [r2, #0x60]
_08009E1A:
	ldr r1, [r2, #0x64]
	ldr r3, [r2, #0x5c]
	cmp r1, r3
	bge _08009E2E
	ldr r0, [r2, #0x58]
	adds r0, r1, r0
	str r0, [r2, #0x64]
	cmp r0, r3
	ble _08009E3E
	b _08009E3C
_08009E2E:
	cmp r1, r3
	ble _08009E3E
	ldr r0, [r2, #0x58]
	subs r0, r1, r0
	str r0, [r2, #0x64]
	cmp r0, r3
	bge _08009E3E
_08009E3C:
	str r3, [r2, #0x64]
_08009E3E:
	adds r1, r2, #0
	adds r1, #0x24
	movs r0, #0
	strb r0, [r1]
	ldr r0, [r2, #0x60]
	cmp r0, #0
	ble _08009E50
	movs r0, #1
	b _08009E56
_08009E50:
	cmp r0, #0
	bge _08009E58
	movs r0, #2
_08009E56:
	strb r0, [r1]
_08009E58:
	ldr r0, [r2, #0x64]
	cmp r0, #0
	ble _08009E62
	movs r0, #8
	b _08009E68
_08009E62:
	cmp r0, #0
	bge _08009E6E
	movs r0, #4
_08009E68:
	ldrb r3, [r1]
	orrs r0, r3
	strb r0, [r1]
_08009E6E:
	ldr r0, [r2]
	ldr r1, [r2, #4]
	str r0, [r2, #0x6c]
	str r1, [r2, #0x70]
	ldr r0, [r2]
	ldr r3, [r2, #0x60]
	adds r0, r0, r3
	str r0, [r2]
	ldr r0, [r2, #4]
	ldr r1, [r2, #0x64]
	adds r0, r0, r1
	str r0, [r2, #4]
	ldr r0, _08009EA4 @ =gUnknown_03001298
	ldr r2, [r0]
	cmp r2, #0
	beq _08009E94
	cmp r1, #0
	bne _08009E94
	str r1, [r0]
_08009E94:
	str r1, [r0]
	movs r0, #0
	cmp r3, #0
	bne _08009EA0
	cmp r1, #0
	beq _08009EA2
_08009EA0:
	movs r0, #1
_08009EA2:
	bx lr
	.align 2, 0
_08009EA4: .4byte gUnknown_03001298
.endif

