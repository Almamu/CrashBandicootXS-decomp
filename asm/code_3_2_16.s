.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_800AAEC
sub_800AAEC: @ 0x0800AAEC
	push {r4, r5, r6, lr}
	sub sp, #0x10
	adds r6, r1, #0
	ldr r1, [r0, #0x20]
	lsls r2, r6, #3
	subs r2, r2, r6
	lsls r2, r2, #2
	ldr r1, [r1]
	adds r1, r1, r2
	adds r1, #4
	adds r4, r1, #0
	ldrb r3, [r4, #5]
	ldr r1, [r0]
	ldr r2, [r0, #4]
	str r1, [sp, #4]
	str r2, [sp, #8]
	ldr r1, [r0, #4]
	str r1, [sp, #0xc]
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	movs r2, #1
	cmp r0, #0
	bge _0800AB1E
	movs r2, #2
_0800AB1E:
	movs r0, #2
	ldrsh r1, [r4, r0]
	lsls r1, r1, #8
	ldr r0, [sp, #8]
	adds r1, r1, r0
	ldr r0, [sp, #4]
	asrs r0, r0, #8
	str r0, [sp, #4]
	asrs r1, r1, #8
	str r1, [sp, #8]
	ldr r0, _0800AB4C @ =gUnknown_03001308
	ldr r0, [r0]
	add r1, sp, #0xc
	str r1, [sp]
	adds r1, r2, #0
	add r2, sp, #4
	bl sub_8026628
	lsls r0, r0, #0x18
	cmp r0, #0
	beq _0800AB50
_0800AB48:
	movs r0, #0
	b _0800AB90
	.align 2, 0
_0800AB4C: .4byte gUnknown_03001308
_0800AB50:
	movs r5, #0
	b _0800AB84
_0800AB54:
	ldr r0, [r0]
	ldr r1, [r0, #8]
	lsls r0, r5, #2
	adds r0, r0, r1
	ldr r4, [r0]
	ldr r1, [r4, #0x18]
	adds r1, #0x48
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r4, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	cmp r0, #3
	bne _0800AB82
	adds r0, r4, #0
	adds r1, r6, #0
	bl sub_800CD00
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	cmp r0, #1
	beq _0800AB48
_0800AB82:
	adds r5, #1
_0800AB84:
	ldr r0, _0800AB98 @ =gUnknown_0300130C
	ldr r1, [r0]
	ldr r1, [r1]
	cmp r5, r1
	blt _0800AB54
	movs r0, #1
_0800AB90:
	add sp, #0x10
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0800AB98: .4byte gUnknown_0300130C
