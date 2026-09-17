.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_802F7B0
sub_802F7B0: @ 0x0802F7B0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x14
	ldr r2, _0802F850 @ =0x040000D4
	str r0, [r2]
	movs r1, #0xa0
	lsls r1, r1, #0x13
	str r1, [r2, #4]
	ldr r1, _0802F854 @ =0x80000100
	str r1, [r2, #8]
	ldr r1, [r2, #8]
	movs r2, #0x80
	lsls r2, r2, #2
	adds r1, r0, r2
	movs r5, #0
	ldrsh r3, [r1, r5]
	mov r8, r3
	adds r2, #2
	adds r1, r0, r2
	movs r5, #0
	ldrsh r3, [r1, r5]
	str r3, [sp]
	movs r1, #0x81
	lsls r1, r1, #2
	adds r0, r0, r1
	ldm r0!, {r2}
	str r2, [sp, #4]
	mov r1, r8
	muls r1, r3, r1
	adds r1, #1
	lsrs r2, r1, #0x1f
	adds r1, r1, r2
	asrs r1, r1, #1
	lsls r1, r1, #2
	adds r1, r0, r1
	str r1, [sp, #8]
	ldr r3, [sp, #4]
	lsls r1, r3, #5
	ldr r5, [sp, #8]
	adds r6, r1, r5
	adds r7, r0, #0
	ldr r5, _0802F858 @ =0x0600D000
	bl sub_8029AC4
	ldr r1, _0802F85C @ =0xFFFFFE00
	adds r1, r0, r1
	str r1, [sp, #0xc]
	movs r2, #0
	mov ip, r2
	movs r0, #0
	ldr r3, [sp]
	cmp r0, r3
	bge _0802F892
_0802F820:
	movs r4, #0
	movs r1, #0x40
	adds r1, r1, r5
	mov sb, r1
	adds r0, #1
	mov sl, r0
	cmp r4, r8
	bge _0802F888
	movs r2, #0xf8
	lsls r2, r2, #3
	adds r3, r5, r2
	adds r2, r5, #0
_0802F838:
	ldrh r5, [r7]
	ldr r0, [sp, #0xc]
	adds r5, r5, r0
	str r5, [sp, #0x10]
	adds r7, #2
	mov r1, ip
	cmp r1, #0
	beq _0802F860
	ldrb r5, [r6]
	lsrs r1, r5, #4
	adds r6, #1
	b _0802F866
	.align 2, 0
_0802F850: .4byte 0x040000D4
_0802F854: .4byte 0x80000100
_0802F858: .4byte 0x0600D000
_0802F85C: .4byte 0xFFFFFE00
_0802F860:
	movs r1, #0xf
	ldrb r0, [r6]
	ands r1, r0
_0802F866:
	movs r0, #1
	mov r5, ip
	eors r5, r0
	mov ip, r5
	lsls r1, r1, #0xc
	ldr r0, [sp, #0x10]
	orrs r1, r0
	cmp r4, #0x1f
	bgt _0802F87C
	strh r1, [r2]
	b _0802F87E
_0802F87C:
	strh r1, [r3]
_0802F87E:
	adds r3, #2
	adds r2, #2
	adds r4, #1
	cmp r4, r8
	blt _0802F838
_0802F888:
	mov r5, sb
	mov r0, sl
	ldr r1, [sp]
	cmp r0, r1
	blt _0802F820
_0802F892:
	movs r2, #0x80
	lsls r2, r2, #0x13
	ldrh r0, [r2]
	movs r3, #0x80
	lsls r3, r3, #2
	adds r1, r3, #0
	orrs r0, r1
	strh r0, [r2]
	ldr r1, _0802F8DC @ =0x0400000A
	ldr r5, _0802F8E0 @ =0x00005A07
	adds r0, r5, #0
	strh r0, [r1]
	bl sub_8029AC4
	lsls r0, r0, #5
	movs r1, #0xc0
	lsls r1, r1, #0x13
	adds r0, r0, r1
	ldr r2, _0802F8E4 @ =0x040000D4
	ldr r3, [sp, #8]
	str r3, [r2]
	str r0, [r2, #4]
	ldr r5, [sp, #4]
	lsls r0, r5, #4
	movs r1, #0x80
	lsls r1, r1, #0x18
	orrs r0, r1
	str r0, [r2, #8]
	ldr r0, [r2, #8]
	add sp, #0x14
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802F8DC: .4byte 0x0400000A
_0802F8E0: .4byte 0x00005A07
_0802F8E4: .4byte 0x040000D4

	thumb_func_start sub_802F8E8
sub_802F8E8: @ 0x0802F8E8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	adds r4, r0, #0
	adds r5, r1, #0
	mov r8, r2
	str r3, [sp]
	ldr r6, _0802F910 @ =0x0600D000
	bl sub_8029AC4
	ldr r1, _0802F914 @ =0xFFFFFE00
	adds r1, r1, r0
	mov sl, r1
	movs r7, #0
	movs r0, #0
	b _0802F966
	.align 2, 0
_0802F910: .4byte 0x0600D000
_0802F914: .4byte 0xFFFFFE00
_0802F918:
	movs r3, #0
	movs r1, #0x40
	adds r1, r1, r6
	mov ip, r1
	adds r0, #1
	mov sb, r0
	cmp r3, r8
	bge _0802F962
	adds r2, r6, #0
_0802F92A:
	ldrh r6, [r5]
	add r6, sl
	adds r5, #2
	cmp r7, #0
	beq _0802F93C
	ldrb r0, [r4]
	lsrs r1, r0, #4
	adds r4, #1
	b _0802F942
_0802F93C:
	movs r1, #0xf
	ldrb r0, [r4]
	ands r1, r0
_0802F942:
	movs r0, #1
	eors r7, r0
	lsls r1, r1, #0xc
	orrs r1, r6
	cmp r3, #0x1f
	bgt _0802F952
	strh r1, [r2]
	b _0802F95A
_0802F952:
	movs r6, #0xf8
	lsls r6, r6, #3
	adds r0, r2, r6
	strh r1, [r0]
_0802F95A:
	adds r2, #2
	adds r3, #1
	cmp r3, r8
	blt _0802F92A
_0802F962:
	mov r6, ip
	mov r0, sb
_0802F966:
	ldr r1, [sp]
	cmp r0, r1
	blt _0802F918
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

