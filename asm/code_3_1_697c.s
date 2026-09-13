.include "asm/macros.inc"

.syntax unified
.arm
	thumb_func_start sub_800697C
sub_800697C: @ 0x0800697C
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	adds r6, r0, #0
	bl sub_800695C
	adds r4, r0, #0
	adds r0, r6, #0
	bl sub_80068CC
	mov sb, r0
	adds r0, r6, #0
	bl sub_8006864
	adds r5, r0, #0
	adds r0, r6, #0
	bl sub_8006820
	mov r8, r0
	adds r0, r6, #0
	bl sub_80067EC
	add r4, sb
	lsrs r1, r5, #0x1f
	adds r5, r5, r1
	asrs r5, r5, #1
	adds r4, r4, r5
	add r4, r8
	adds r4, r4, r0
	ldrb r1, [r6, #2]
	lsrs r0, r1, #7
	adds r4, r4, r0
	lsls r0, r1, #0x1a
	lsrs r0, r0, #0x1f
	adds r4, r4, r0
	lsls r0, r1, #0x19
	lsrs r0, r0, #0x1f
	adds r4, r4, r0
	lsls r1, r1, #0x1b
	lsrs r1, r1, #0x1f
	adds r4, r4, r1
	movs r0, #0x64
	muls r0, r4, r0
	movs r1, #0x48
	bl sub_803ADB4
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
