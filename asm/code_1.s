.include "asm/macros.inc"

.syntax unified
.arm

.if NON_MATCHING == 0
	thumb_func_start mem_collect
mem_collect: @ 0x08000274
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	cmp r7, #0
	bge _08000304
	ldr r0, _080002AC @ =mem_iwram_heap_pointer
	ldr r5, [r0]
	ldr r1, [r5, #8]
	mov r8, r0
	cmp r1, r5
	beq _08000304
	ldr r0, _080002B0 @ =mem_ewram_heap_pointer
	mov ip, r0
_08000290:
	ldr r6, [r1, #8]
	ldr r0, [r1, #4]
	cmp r0, #2
	bne _080002FE
	adds r0, r1, #0
	adds r0, #0x10
	cmp r0, #0
	beq _080002FE
	mov r1, r8
	ldr r2, [r1]
	cmp r0, r2
	blo _080002B4
	adds r4, r2, #0
	b _080002B8
	.align 2, 0
_080002AC: .4byte mem_iwram_heap_pointer
_080002B0: .4byte mem_ewram_heap_pointer
_080002B4:
	mov r1, ip
	ldr r4, [r1]
_080002B8:
	adds r3, r0, #0
	subs r3, #0x10
	movs r0, #0
	str r0, [r3, #4]
	ldr r2, [r3, #0xc]
	ldr r0, [r2, #4]
	cmp r0, #0
	bne _080002E0
	ldr r0, [r2]
	ldr r1, [r3]
	adds r0, r0, r1
	str r0, [r2]
	ldr r0, [r3, #8]
	str r0, [r2, #8]
	str r2, [r0, #0xc]
	ldr r0, [r4, #0x10]
	cmp r3, r0
	bne _080002DE
	str r2, [r4, #0x10]
_080002DE:
	adds r3, r2, #0
_080002E0:
	ldr r2, [r3, #8]
	ldr r0, [r2, #4]
	cmp r0, #0
	bne _080002FE
	ldr r0, [r3]
	ldr r1, [r2]
	adds r0, r0, r1
	str r0, [r3]
	ldr r0, [r2, #8]
	str r0, [r3, #8]
	str r3, [r0, #0xc]
	ldr r0, [r4, #0x10]
	cmp r2, r0
	bne _080002FE
	str r3, [r4, #0x10]
_080002FE:
	adds r1, r6, #0
	cmp r1, r5
	bne _08000290
_08000304:
	movs r0, #0x80
	lsls r0, r0, #0x17
	ands r0, r7
	cmp r0, #0
	beq _08000390
	ldr r0, _08000338 @ =mem_ewram_heap_pointer
	ldr r5, [r0]
	ldr r1, [r5, #8]
	mov r8, r0
	cmp r1, r5
	beq _08000390
	ldr r7, _0800033C @ =mem_iwram_heap_pointer
_0800031C:
	ldr r6, [r1, #8]
	ldr r0, [r1, #4]
	cmp r0, #2
	bne _0800038A
	adds r0, r1, #0
	adds r0, #0x10
	cmp r0, #0
	beq _0800038A
	ldr r2, [r7]
	cmp r0, r2
	blo _08000340
	adds r4, r2, #0
	b _08000344
	.align 2, 0
_08000338: .4byte mem_ewram_heap_pointer
_0800033C: .4byte mem_iwram_heap_pointer
_08000340:
	mov r1, r8
	ldr r4, [r1]
_08000344:
	adds r3, r0, #0
	subs r3, #0x10
	movs r0, #0
	str r0, [r3, #4]
	ldr r2, [r3, #0xc]
	ldr r0, [r2, #4]
	cmp r0, #0
	bne _0800036C
	ldr r0, [r2]
	ldr r1, [r3]
	adds r0, r0, r1
	str r0, [r2]
	ldr r0, [r3, #8]
	str r0, [r2, #8]
	str r2, [r0, #0xc]
	ldr r0, [r4, #0x10]
	cmp r3, r0
	bne _0800036A
	str r2, [r4, #0x10]
_0800036A:
	adds r3, r2, #0
_0800036C:
	ldr r2, [r3, #8]
	ldr r0, [r2, #4]
	cmp r0, #0
	bne _0800038A
	ldr r0, [r3]
	ldr r1, [r2]
	adds r0, r0, r1
	str r0, [r3]
	ldr r0, [r2, #8]
	str r0, [r3, #8]
	str r3, [r0, #0xc]
	ldr r0, [r4, #0x10]
	cmp r2, r0
	bne _0800038A
	str r3, [r4, #0x10]
_0800038A:
	adds r1, r6, #0
	cmp r1, r5
	bne _0800031C
_08000390:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
.endif
	.align 2, 0
_0800039C:
	.byte 0x03, 0x1C, 0x00, 0x2B
	.byte 0x0A, 0xDA, 0x02, 0x48, 0x02, 0x68, 0x91, 0x68, 0x03, 0xE0, 0x00, 0x00, 0xCC, 0x07, 0x00, 0x03
	.byte 0x89, 0x68, 0x88, 0x68, 0x90, 0x42, 0xFB, 0xD1, 0x80, 0x20, 0xC0, 0x05, 0x18, 0x40, 0x00, 0x28
	.byte 0x0A, 0xD0, 0x02, 0x48, 0x02, 0x68, 0x91, 0x68, 0x03, 0xE0, 0x00, 0x00, 0xD0, 0x07, 0x00, 0x03
	.byte 0x89, 0x68, 0x88, 0x68, 0x90, 0x42, 0xFB, 0xD1, 0x70, 0x47, 0x00, 0x00

.if NON_MATCHING == 0
	thumb_func_start mem_free_bytes
mem_free_bytes: @ 0x080003DC
	push {r4, r5, lr}
	adds r5, r0, #0
	movs r4, #0
	cmp r5, #0
	bge _08000406
	ldr r0, _08000438 @ =mem_iwram_heap_pointer
	ldr r3, [r0]
	movs r1, #0
	ldr r2, [r3, #8]
	cmp r2, r3
	beq _08000404
_080003F2:
	ldr r0, [r2, #4]
	cmp r0, #0
	bne _080003FE
	subs r1, #0x10
	ldr r0, [r2]
	adds r1, r1, r0
_080003FE:
	ldr r2, [r2, #8]
	cmp r2, r3
	bne _080003F2
_08000404:
	adds r4, r4, r1
_08000406:
	movs r0, #0x80
	lsls r0, r0, #0x17
	ands r0, r5
	cmp r0, #0
	beq _08000430
	ldr r0, _0800043C @ =mem_ewram_heap_pointer
	ldr r3, [r0]
	movs r1, #0
	ldr r2, [r3, #8]
	cmp r2, r3
	beq _0800042E
_0800041C:
	ldr r0, [r2, #4]
	cmp r0, #0
	bne _08000428
	subs r1, #0x10
	ldr r0, [r2]
	adds r1, r1, r0
_08000428:
	ldr r2, [r2, #8]
	cmp r2, r3
	bne _0800041C
_0800042E:
	adds r4, r4, r1
_08000430:
	adds r0, r4, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08000438: .4byte mem_iwram_heap_pointer
_0800043C: .4byte mem_ewram_heap_pointer
.endif
