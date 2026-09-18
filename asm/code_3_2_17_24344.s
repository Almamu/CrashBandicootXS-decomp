.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8024344
sub_8024344: @ 0x08024344
	push {r4, r5, r6, r7, lr}
	mov ip, r1
	movs r3, #0
	ldr r2, _080243DC @ =gStaticData_0816C86C
	lsls r1, r0, #3
	adds r1, r1, r0
	lsls r1, r1, #2
	adds r2, #0x20
	adds r1, r1, r2
	ldr r5, [r1]
	movs r4, #0
	ldr r0, [r5]
	cmp r3, r0
	bge _0802438C
	ldr r2, [r5, #4]
	mov r1, ip
	lsls r7, r1, #1
	adds r6, r0, #0
_08024368:
	ldr r1, [r2]
	ldr r0, [r1, #8]
	cmp r0, #3
	beq _08024380
	ldr r0, [r1, #4]
	ldr r0, [r0, #0x1c]
	ldr r1, [r0, #0x10]
	adds r1, r7, r1
	ldrh r3, [r1]
	rsbs r0, r3, #0
	orrs r0, r3
	lsrs r3, r0, #0x1f
_08024380:
	adds r2, #4
	adds r4, #1
	cmp r4, r6
	bge _0802438C
	cmp r3, #0
	beq _08024368
_0802438C:
	cmp r3, #0
	bne _080243D4
	ldr r1, [r5, #8]
	cmp r1, #0
	beq _080243B0
	ldr r0, [r1, #8]
	cmp r0, #3
	beq _080243B0
	ldr r0, [r1, #4]
	ldr r0, [r0, #0x1c]
	ldr r0, [r0, #0x10]
	mov r2, ip
	lsls r1, r2, #1
	adds r1, r1, r0
	ldrh r3, [r1]
	rsbs r0, r3, #0
	orrs r0, r3
	lsrs r3, r0, #0x1f
_080243B0:
	cmp r3, #0
	bne _080243D4
	ldr r1, [r5, #0xc]
	cmp r1, #0
	beq _080243D4
	ldr r0, [r1, #8]
	cmp r0, #3
	beq _080243D4
	ldr r0, [r1, #4]
	ldr r0, [r0, #0x1c]
	ldr r0, [r0, #0x10]
	mov r2, ip
	lsls r1, r2, #1
	adds r1, r1, r0
	ldrh r3, [r1]
	rsbs r0, r3, #0
	orrs r0, r3
	lsrs r3, r0, #0x1f
_080243D4:
	adds r0, r3, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080243DC: .4byte gStaticData_0816C86C

