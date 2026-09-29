@ Frozen disassembly of the IWRAM image's ARM code (IWRAM 0x03000000-
@ 0x030007CC, stored in ROM at 0x087E55E4-0x087E5DB0), the objdiff target
@ for the IWRAM code units in tools/report_units.py. Generated once from
@ baserom.gba (objdump -marm, branch targets and literal pools turned into
@ labels, the pool words into the symbols they point at) and never edited
@ afterwards, like expected/code_3.s and expected/legacy.s - see
@ expected/README.md and docs/decomp_dev.md. Addresses are IWRAM (run)
@ addresses.

.include "asm/macros.inc"

.syntax unified
.arm

	arm_func_start IntrMain
IntrMain: @ 0x03000000
	mov r12, #67108864
	add r3, r12, #512
	ldr r2, [r3]
	and r1, r2, r2, lsr #16
	mov r2, #0
	ands r0, r1, #1
	strhne r0, [r12, #-8]
	bne _030000BC
	add r2, r2, #4
	ands r0, r1, #2
	bne _030000BC
	add r2, r2, #4
	ands r0, r1, #4
	bne _030000BC
	add r2, r2, #4
	ands r0, r1, #8
	bne _030000BC
	add r2, r2, #4
	ands r0, r1, #16
	bne _030000BC
	add r2, r2, #4
	ands r0, r1, #32
	bne _030000BC
	add r2, r2, #4
	ands r0, r1, #64
	bne _030000BC
	add r2, r2, #4
	ands r0, r1, #128
	bne _030000BC
	add r2, r2, #4
	ands r0, r1, #256
	bne _030000BC
	add r2, r2, #4
	ands r0, r1, #512
	bne _030000BC
	add r2, r2, #4
	ands r0, r1, #1024
	bne _030000BC
	add r2, r2, #4
	ands r0, r1, #2048
	bne _030000BC
	add r2, r2, #4
	ands r0, r1, #4096
	bne _030000BC
	add r2, r2, #4
	ands r0, r1, #8192
_030000B8:
	bne _030000B8
_030000BC:
	strh r0, [r3, #2]
	ldr r1, _030000D0 @ =gUnknown_030009E8
	add r1, r1, r2
	ldr r0, [r1]
	bx r0
_030000D0: .4byte gUnknown_030009E8

	arm_func_start strlen_arm
strlen_arm: @ 0x030000D4
	mov r2, #0
	ldrb r3, [r0, r2]
	cmp r3, r2
	beq _030000F4
_030000E4:
	add r2, r2, #1
	ldrb r3, [r0, r2]
	cmp r3, #0
	bne _030000E4
_030000F4:
	mov r0, r2
	bx lr

	arm_func_start strcpy_arm
strcpy_arm: @ 0x030000FC
	ldrb r3, [r1]
	cmp r3, #0
	beq _03000118
_03000108:
	strb r3, [r0], #1
	ldrb r3, [r1, #1]!
	cmp r3, #0
	bne _03000108
_03000118:
	strb r3, [r0]
	bx lr

	arm_func_start strncpy_arm
strncpy_arm: @ 0x03000120
	cmp r2, #0
	beq _03000158
	ldrb r3, [r1]
	cmp r3, #0
	beq _0300014C
_03000134:
	strb r3, [r0], #1
	subs r2, r2, #1
	beq _0300014C
	ldrb r3, [r1, #1]!
	cmp r3, #0
	bne _03000134
_0300014C:
	mov r3, #0
	cmp r2, r3
	strbne r3, [r0]
_03000158:
	bx lr

	arm_func_start strcat_arm
strcat_arm: @ 0x0300015C
	ldrb r3, [r0]
	cmp r3, #0
	beq _03000174
_03000168:
	ldrb r3, [r0, #1]!
	cmp r3, #0
	bne _03000168
_03000174:
	ldrb r3, [r1]
	cmp r3, #0
	beq _03000190
_03000180:
	strb r3, [r0], #1
	ldrb r3, [r1, #1]!
	cmp r3, #0
	bne _03000180
_03000190:
	strb r3, [r0]
	bx lr

	arm_func_start itoa_arm
itoa_arm: @ 0x03000198
	push {r4, r5, r6}
	mov r5, #0
	cmp r0, #0
	movge r4, #0
	movlt r4, #1
	rsblt r0, r0, #0
	mov r6, r1
	mov r12, r2
	cmp r2, #16
	beq _030001E0
_030001C0:
	mov r1, r12
	svc 0x00060000
	add r1, r1, #48
	strb r1, [r6, r5]
	add r5, r5, #1
	cmp r0, #0
	bne _030001C0
	b _03000204
_030001E0:
	and r1, r0, #15
	asr r0, r0, #4
	cmp r1, #10
	addge r1, r1, #55
	addlt r1, r1, #48
	strb r1, [r6, r5]
	add r5, r5, #1
	cmp r0, #0
	bne _030001E0
_03000204:
	cmp r4, #0
	movne r4, #45
	strbne r4, [r6, r5]
	addne r5, r5, #1
	movne r4, #0
	strb r4, [r6, r5]
	sub r1, r5, #1
_03000220:
	ldrb r3, [r6, r4]
	ldrb r0, [r6, r1]
	strb r3, [r6, r1]
	strb r0, [r6, r4]
	add r4, r4, #1
	sub r1, r1, #1
	cmp r4, r1
	blt _03000220
	mov r0, r5
	pop {r4, r5, r6}
	bx lr

	arm_func_start UnpackNibbleTiles
UnpackNibbleTiles: @ 0x0300024C
	push {r4, r5, r6, r7, lr}
	cmp r1, #0
	sub sp, sp, #4
	mov r4, r0
	movne lr, #100663296
	addne lr, lr, #32768
	moveq lr, #100663296
	addeq lr, lr, #40960
	add r6, lr, #6400
	cmp lr, r6
	beq _03000360
	mov r0, #212
	add r0, r0, #67108864
	mov r3, #0
	ldrh r1, [r4], #2
	add r2, sp, #2
	strh r3, [sp, #2]
	mov r7, r2
	str r2, [r0]
	lsl r3, r1, #1
	str lr, [r0, #4]
	orr r3, r3, #-2130706432
	str r3, [r0, #8]
	add lr, lr, r1, lsl #2
	ldr r3, [r0, #8]
	cmp lr, r6
	beq _03000360
	mov r5, r0
_030002BC:
	ldrh r1, [r4], #2
	cmp r1, #0
	beq _03000328
	mov r12, r1
_030002CC:
	ldrh r3, [r4], #2
	ands r0, r3, #15
	orrne r0, r0, #240
	moveq r0, #0
	lsl r3, r3, #16
	lsr r2, r3, #20
	ands r2, r2, #15
	orrne r2, r2, #240
	moveq r2, #0
	lsr r1, r3, #24
	ands r1, r1, #15
	orrne r1, r1, #240
	moveq r1, #0
	lsrs r3, r3, #28
	orrne r3, r3, #240
	moveq r3, #0
	orr r0, r0, r3, lsl #24
	lsl r2, r2, #8
	orr r2, r2, r1, lsl #16
	orr r0, r0, r2
	str r0, [lr], #4
	subs r12, r12, #1
	bne _030002CC
_03000328:
	cmp lr, r6
	beq _03000360
	mov r3, #0
	strh r3, [sp, #2]
	ldrh r1, [r4], #2
	str r7, [r5]
	lsl r3, r1, #1
	str lr, [r5, #4]
	orr r3, r3, #-2130706432
	str r3, [r5, #8]
	add lr, lr, r1, lsl #2
	ldr r3, [r5, #8]
	cmp lr, r6
	bne _030002BC
_03000360:
	add sp, sp, #4
	pop {r4, r5, r6, r7, lr}
	bx lr

	arm_func_start DrawMirroredTilemap
DrawMirroredTilemap: @ 0x0300036C
	push {r4, r5, r6, r7, r8, r9, r10, lr}
	sub sp, sp, #12
	str r0, [sp, #8]
	cmp r1, #0
	str r3, [sp, #4]
	mov r7, r2
	movne lr, #100663296
	addne lr, lr, #57344
	moveq lr, #100663296
	addeq lr, lr, #61440
	cmp r1, #0
	ldr r2, [sp, #4]
	moveq r5, #256
	movne r5, #0
	lsl r3, r2, #7
	sub r3, r3, #64
	add r6, lr, r3
	mov r3, #0
	cmp r3, r2
	bge _03000468
_030003BC:
	mov r4, #0
	cmp r4, r7
	add r9, lr, #64
	add r3, r3, #1
	str r3, [sp]
	sub r10, r6, #64
	bge _03000450
	lsl r8, r7, #1
_030003DC:
	tst r5, #1
	beq _030003F8
	ldr r2, [sp, #8]
	ldrb r3, [r2], #1
	str r2, [sp, #8]
	lsr r12, r3, #4
	b _03000404
_030003F8:
	ldr r2, [sp, #8]
	ldrb r3, [r2]
	and r12, r3, #15
_03000404:
	orr r12, r5, r12, lsl #12
	add r5, r5, #1
	add r3, r4, #1
	rsb r1, r3, r8
	lsl r0, r4, #1
	mov r4, r3
	orr r3, r12, #2048
	add r2, r1, #992
	strh r12, [r0, lr]
	cmp r1, #31
	strh r3, [r0, r6]
	lslle r1, r1, #1
	lslgt r1, r2, #1
	orr r3, r12, #1024
	strh r3, [r1, lr]
	orr r2, r12, #3072
	strh r2, [r1, r6]
	cmp r4, r7
	blt _030003DC
_03000450:
	ldr r3, [sp]
	mov lr, r9
	ldr r2, [sp, #4]
	mov r6, r10
	cmp r3, r2
	blt _030003BC
_03000468:
	add sp, sp, #12
	pop {r4, r5, r6, r7, r8, r9, r10, lr}
	bx lr

	arm_func_start HeapSortActorsByKey
HeapSortActorsByKey: @ 0x03000474
	push {r4, r5, r6, r7, r8, r9, r10, lr}
	mov r7, r0
	add r3, r7, r7, lsr #31
	asr r8, r3, #1
	cmp r8, #0
	mov lr, r1
	ble _03000554
_03000490:
	sub r4, r8, #1
	mov r8, r4
	lsl r3, r4, #1
	add r12, r3, #1
	cmp r12, r7
	bge _0300054C
	mov r9, #1
	mov r10, #0
_030004B0:
	add r3, r12, #1
	ldr r6, [lr, r12, lsl #2]
	cmp r3, r7
	ldr r5, [lr, r4, lsl #2]
	mov r0, r3
	bge _03000510
	ldr r3, [lr, r0, lsl #2]
	ldr r2, [r6, #20]
	ldr r1, [r3, #20]
	cmp r1, r2
	mov r3, r9
	movls r3, r10
	ldr r5, [lr, r4, lsl #2]
	cmp r3, #0
	beq _03000510
	ldr r3, [lr, r0, lsl #2]
	ldr r2, [r5, #20]
	ldr r1, [r3, #20]
	cmp r1, r2
	mov r3, r9
	movls r3, r10
	cmp r3, #0
	movne r12, r0
	bne _0300052C
_03000510:
	ldr r2, [r6, #20]
	ldr r3, [r5, #20]
	cmp r2, r3
	mov r3, r9
	movls r3, r10
	cmp r3, #0
	beq _0300054C
_0300052C:
	ldr r3, [lr, r12, lsl #2]
	str r3, [lr, r4, lsl #2]
	mov r4, r12
	str r5, [lr, r12, lsl #2]
	lsl r3, r4, #1
	add r12, r3, #1
	cmp r12, r7
	blt _030004B0
_0300054C:
	cmp r8, #0
	bgt _03000490
_03000554:
	cmp r7, #1
	ble _0300062C
_0300055C:
	ldr r2, [lr]
	sub r7, r7, #1
	ldr r3, [lr, r7, lsl #2]
	mov r4, #0
	str r3, [lr]
	add r12, r4, #1
	str r2, [lr, r7, lsl #2]
	cmp r12, r7
	bge _03000624
	mov r10, r12
	mov r8, r4
_03000588:
	add r3, r12, #1
	ldr r6, [lr, r12, lsl #2]
	cmp r3, r7
	ldr r5, [lr, r4, lsl #2]
	mov r0, r3
	bge _030005E8
	ldr r3, [lr, r0, lsl #2]
	ldr r2, [r6, #20]
	ldr r1, [r3, #20]
	cmp r1, r2
	mov r3, r10
	movls r3, r8
	ldr r5, [lr, r4, lsl #2]
	cmp r3, #0
	beq _030005E8
	ldr r3, [lr, r0, lsl #2]
	ldr r2, [r5, #20]
	ldr r1, [r3, #20]
	cmp r1, r2
	mov r3, r10
	movls r3, r8
	cmp r3, #0
	movne r12, r0
	bne _03000604
_030005E8:
	ldr r2, [r6, #20]
	ldr r3, [r5, #20]
	cmp r2, r3
	mov r3, r10
	movls r3, r8
	cmp r3, #0
	beq _03000624
_03000604:
	ldr r3, [lr, r12, lsl #2]
	str r3, [lr, r4, lsl #2]
	mov r4, r12
	str r5, [lr, r12, lsl #2]
	lsl r3, r4, #1
	add r12, r3, #1
	cmp r12, r7
	blt _03000588
_03000624:
	cmp r7, #1
	bgt _0300055C
_0300062C:
	pop {r4, r5, r6, r7, r8, r9, r10, lr}
	bx lr

	arm_func_start UnpackRleSpriteFrame
UnpackRleSpriteFrame: @ 0x03000634
	push {r4, r5, r6, r7, lr}
	sub sp, sp, #4
	mov r4, r1
	ldrb r3, [r4, #1]
	ldrb r2, [r4]
	mul r1, r2, r3
	add r5, r0, r1, lsl #5
	cmp r0, r5
	beq _030006F0
	mov lr, #212
	add lr, lr, #67108864
	mov r6, #0
	ldrh r12, [r4, #4]
	add r2, sp, #2
	strh r6, [sp, #2]
	add r1, r4, #6
	str r2, [lr]
	mov r7, r2
	str r0, [lr, #4]
	orr r3, r12, #-2130706432
	str r3, [lr, #8]
	add r0, r0, r12, lsl #1
	ldr r3, [lr, #8]
	cmp r0, r5
	beq _030006F0
	mov r4, r6
_0300069C:
	ldrh r12, [r1], #2
	lsl r3, r12, #1
	str r1, [lr]
	orr r2, r12, #-2147483648
	str r0, [lr, #4]
	add r1, r1, r3
	str r2, [lr, #8]
	add r0, r0, r3
	ldr r2, [lr, #8]
	cmp r0, r5
	beq _030006F0
	strh r4, [sp, #2]
	str r7, [lr]
	ldrh r12, [r1], #2
	str r0, [lr, #4]
	orr r3, r12, #-2130706432
	str r3, [lr, #8]
	add r0, r0, r12, lsl #1
	ldr r3, [lr, #8]
	cmp r0, r5
	bne _0300069C
_030006F0:
	add sp, sp, #4
	pop {r4, r5, r6, r7, lr}
	bx lr

	arm_func_start LookupSpriteFrameCache
LookupSpriteFrameCache: @ 0x030006FC
	stmfd sp!, {lr}
	ldr r3, _030007C4 @ =gUnknown_03001354
	ldr r12, [r3]
	cmp r12, r3
	beq _03000744
	mov r2, r3
_03000714:
	ldr r3, [r12, #8]
	cmp r3, r0
	bne _03000738
	ldr r0, [r12, #12]
	add r0, r0, #-117440512
	add r0, r0, #16711680
	lsr r0, r0, #5
	ldmfd sp!, {lr}
	bx lr
_03000738:
	ldr r12, [r12]
	cmp r12, r2
	bne _03000714
_03000744:
	ldr r3, _030007C8 @ =gUnknown_03001364
	ldr r12, [r3]
	cmp r12, r3
	beq _030007B8
	mov r2, r3
	ldr lr, _030007C4 @ =gUnknown_03001354
_0300075C:
	ldr r3, [r12, #8]
	cmp r3, r0
	bne _030007AC
	ldr r2, [r12, #4]
	ldr r3, [r12]
	ldr r0, [r12, #12]
	str r3, [r2]
	ldm r12, {r1, r3}
	str r3, [r1, #4]
	str lr, [r12, #4]
	ldr r3, [lr]
	str r3, [r12]
	ldr r3, [lr]
	add r0, r0, #-117440512
	str r12, [r3, #4]
	add r0, r0, #16711680
	str r12, [lr]
	lsr r0, r0, #5
	ldmfd sp!, {lr}
	bx lr
_030007AC:
	ldr r12, [r12]
	cmp r12, r2
	bne _0300075C
_030007B8:
	mvn r0, #0
	ldmfd sp!, {lr}
	bx lr
_030007C4: .4byte gUnknown_03001354
_030007C8: .4byte gUnknown_03001364
