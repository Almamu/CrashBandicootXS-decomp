.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8034058
sub_8034058: @ 0x08034058
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	adds r5, r0, #0
	adds r6, r1, #0
	adds r7, r2, #0
	mov r8, r3
	add r0, sp, #0x24
	ldrb r0, [r0]
	mov sb, r0
	bl sub_80338DC
	movs r4, #0x10
	cmp r0, #0
	bne _0803407C
	movs r4, #0x18
_0803407C:
	ldr r0, [sp, #0x20]
	str r0, [sp]
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r7, #0
	mov r3, r8
	bl InitActorPart
	str r4, [r5, #0x54]
	ldr r0, _080340D4 @ =gStaticData_087E5554
	str r0, [r5, #0x50]
	adds r3, r5, #0
	adds r3, #0x59
	movs r4, #0
	mov r0, sb
	strb r0, [r3]
	str r4, [r5, #0x28]
	ldrb r0, [r3]
	movs r2, #1
	cmp r0, #0
	beq _080340A8
	movs r2, #0
_080340A8:
	str r4, [r5, #0x28]
	str r4, [r5, #0x44]
	str r2, [r5, #0xc]
	ldr r1, [r5]
	lsls r0, r2, #1
	adds r0, r0, r2
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrh r0, [r0]
	movs r1, #0
	strh r0, [r5, #0x10]
	strb r1, [r5, #0x12]
	str r4, [r5, #8]
	adds r0, r5, #0
	adds r0, #0x58
	strb r1, [r0]
	ldrb r0, [r3]
	cmp r0, #0
	beq _080340DC
	ldr r0, _080340D8 @ =0xFFFFBF00
	b _080340E0
	.align 2, 0
_080340D4: .4byte gStaticData_087E5554
_080340D8: .4byte 0xFFFFBF00
_080340DC:
	movs r0, #0x84
	lsls r0, r0, #8
_080340E0:
	str r0, [r5, #0x5c]
	movs r0, #0xa0
	lsls r0, r0, #4
	str r0, [r5, #0x60]
	movs r0, #1
	rsbs r0, r0, #0
	str r0, [r5, #0x64]
	adds r0, r5, #0
	adds r0, #0x2c
	movs r4, #0
	strb r4, [r0]
	bl sub_80338C4
	ldr r0, [r0, #4]
	str r0, [r5, #0x68]
	str r4, [r5, #0x6c]
	adds r0, r5, #0
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
