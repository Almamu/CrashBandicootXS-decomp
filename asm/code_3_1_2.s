.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8000CBC is reconstructed (but not yet byte-matching) as C in
@ src/printf_util.c, guarded by #if NON_MATCHING - this raw version is
@ only assembled for the default (matching) build. See docs/graphics.md,
@ "Parked, not matched: sub_8000CBC".
.if NON_MATCHING == 0
	thumb_func_start sub_8000CBC
sub_8000CBC: @ 0x08000CBC
	push {r4, r5, r6, r7, lr}
	adds r7, r2, #0
	mov ip, r1
	adds r5, r0, #0
	ldrb r6, [r1]
	movs r0, #1
	add ip, r0
	cmp r6, #0
	beq _08000D12
	cmp r7, #0
	beq _08000CEA
	adds r0, r6, #0
	subs r0, #0x41
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #0x19
	bhi _08000CE4
	adds r0, r6, #0
	adds r0, #0x20
	b _08000CE6
_08000CE4:
	adds r0, r6, #0
_08000CE6:
	lsls r0, r0, #0x18
	lsrs r6, r0, #0x18
_08000CEA:
	ldrb r3, [r5]
	adds r5, #1
	cmp r7, #0
	beq _08000D0A
	adds r0, r3, #0
	subs r0, #0x41
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #0x19
	bhi _08000D04
	adds r0, r3, #0
	adds r0, #0x20
	b _08000D06
_08000D04:
	adds r0, r3, #0
_08000D06:
	lsls r0, r0, #0x18
	lsrs r3, r0, #0x18
_08000D0A:
	cmp r3, r6
	beq _08000D16
	cmp r3, #0
	bne _08000CEA
_08000D12:
	movs r0, #0
	b _08000D62
_08000D16:
	adds r4, r5, #0
	mov r2, ip
_08000D1A:
	ldrb r3, [r2]
	adds r2, #1
	cmp r3, #0
	beq _08000D60
	ldrb r1, [r4]
	adds r4, #1
	cmp r7, #0
	beq _08000D5A
	adds r0, r3, #0
	subs r0, #0x41
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #0x19
	bhi _08000D3C
	adds r0, r3, #0
	adds r0, #0x20
	b _08000D3E
_08000D3C:
	adds r0, r3, #0
_08000D3E:
	lsls r0, r0, #0x18
	lsrs r3, r0, #0x18
	adds r0, r1, #0
	subs r0, #0x41
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #0x19
	bhi _08000D54
	adds r0, r1, #0
	adds r0, #0x20
	b _08000D56
_08000D54:
	adds r0, r1, #0
_08000D56:
	lsls r0, r0, #0x18
	lsrs r1, r0, #0x18
_08000D5A:
	cmp r3, r1
	beq _08000D1A
	b _08000CEA
_08000D60:
	subs r0, r5, #1
_08000D62:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
.endif
