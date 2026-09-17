.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8025E98
sub_8025E98: @ 0x08025E98
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	bl sub_8024E68
	ldr r0, [r5]
	adds r1, r0, #0
	cmp r0, #0
	bge _08025EAE
	adds r1, r0, #7
_08025EAE:
	asrs r1, r1, #3
	mov r8, r1
	adds r2, r0, #0
	adds r2, #0xef
	cmp r2, #0
	bge _08025EBC
	adds r2, #7
_08025EBC:
	asrs r7, r2, #3
	ldr r0, [r5, #4]
	adds r1, r0, #0
	cmp r0, #0
	bge _08025EC8
	adds r1, r0, #7
_08025EC8:
	asrs r6, r1, #3
	adds r4, r0, #0
	adds r4, #0x9f
	cmp r4, #0
	bge _08025ED4
	adds r4, #7
_08025ED4:
	asrs r4, r4, #3
	ldr r1, [r5, #0x30]
	adds r1, #0x48
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	adds r1, r6, #0
	adds r2, r4, #0
	bl sub_803AD84
	ldr r1, [r5, #0x30]
	adds r1, #0x40
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r3, [r1, #4]
	mov r1, r8
	adds r2, r7, #0
	bl sub_803AD84
	ldr r0, [r5, #0x2c]
	adds r1, r5, #0
	bl sub_8024AA0
	adds r0, r5, #0
	mov r1, r8
	adds r2, r7, #0
	bl sub_8025E2C
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r4, #0
	bl sub_8025DE8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

