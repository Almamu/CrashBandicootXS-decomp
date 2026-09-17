.include "asm/macros.inc"

.syntax unified
.arm
	thumb_func_start sub_803B4EC
sub_803B4EC: @ 0x0803B4EC
	push	{r4, r5, lr}
	adds	r4, r0, #0
	movs	r5, #1
	str	r5, [r4, #20]
	ldrb	r2, [r4, #18]
	cmp	r2, #0
	beq _0803B510
	cmp	r4, #0
	beq _0803B546
	ldr	r1, [r4, #80]	@ 0x50
	movs	r2, #8
	ldrsh	r0, [r1, r2]
	adds	r0, r4, r0
	ldr	r2, [r1, #12]
	movs	r1, #3
	bl sub_803AD80
	b _0803B546
_0803B510:
	movs	r3, #16
	ldrsh	r1, [r4, r3]
	ldr	r0, [r4, #8]
	adds	r0, r0, r1
	str	r0, [r4, #8]
	strb	r2, [r4, #18]
	adds	r0, r4, #0
	bl GetAnimFrameBaseOffset
	ldr	r2, [r4, #12]
_0803B524: .4byte 0x516823
	adds	r1, r1, r2
	lsls	r1, r1, #2
	adds	r1, r1, r3
	movs	r3, #4
	ldrsh	r2, [r1, r3]
	cmp	r0, r2
	blt _0803B546
	movs	r0, #6
	ldrsh	r1, [r1, r0]
	subs	r1, r2, r1
	lsls	r1, r1, #8
	ldr	r0, [r4, #8]
	subs	r0, r0, r1
	str	r0, [r4, #8]
	strb	r5, [r4, #18]
_0803B546:
	pop	{r4, r5}
	pop	{r0}
	bx	r0
	movs	r0, #1
	bx	lr
_0803B550: .4byte 0x1c03b500
	ldr r0, _0803B578
	str	r0, [r3, #80]	@ 0x50
	ldr	r2, [r3, #76]	@ 0x4c
	ldr	r0, [r3, #72]	@ 0x48
	str	r0, [r2, #72]	@ 0x48
	ldr	r2, [r3, #72]	@ 0x48
	ldr	r0, [r3, #76]	@ 0x4c
	str	r0, [r2, #76]	@ 0x4c
	movs	r0, #1
	ands	r0, r1
	cmp	r0, #0
	beq _0803B572
	adds	r0, r3, #0
	bl mem_free
_0803B572:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B578: .4byte gStaticData_087E4DF4
_0803B57C: .4byte 0x1c02b500
	ldr	r0, [r2, #36]	@ 0x24
	adds	r0, #170	@ 0xaa
	str	r0, [r2, #36]	@ 0x24
	ldrb	r0, [r2, #18]
	cmp	r0, #0
	beq _0803B5A2
	cmp	r2, #0
	beq _0803B5A8
	ldr	r1, [r2, #80]	@ 0x50
	movs	r3, #8
	ldrsh	r0, [r1, r3]
	adds	r0, r2, r0
	ldr	r2, [r1, #12]
	movs	r1, #3
	bl sub_803AD80
	b _0803B5A8
_0803B5A2:
	adds	r0, r2, #0
	bl sub_802A7B8
_0803B5A8: .4byte 0x4700bc01
	movs	r0, #1
	bx	lr

	thumb_func_start sub_803B5B0
sub_803B5B0: @ 0x0803B5B0
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B5D8
	str	r0, [r3, #80]	@ 0x50
	ldr	r2, [r3, #76]	@ 0x4c
	ldr	r0, [r3, #72]	@ 0x48
	str	r0, [r2, #72]	@ 0x48
	ldr	r2, [r3, #72]	@ 0x48
	ldr	r0, [r3, #76]	@ 0x4c
	str	r0, [r2, #76]	@ 0x4c
	movs	r0, #1
	ands	r0, r1
	cmp	r0, #0
	beq _0803B5D2
	adds	r0, r3, #0
	bl mem_free
_0803B5D2:
	pop	{r0}
_0803B5D4: .4byte 0x4700
_0803B5D8: .4byte gStaticData_087E4DF4
	ldr	r0, [r0, #84]	@ 0x54
	bx	lr
	bx	lr
	movs	r0, r0
	movs	r0, #0
	bx	lr

	thumb_func_start sub_803B5E8
sub_803B5E8: @ 0x0803B5E8
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B610
	str	r0, [r3, #80]	@ 0x50
	ldr	r2, [r3, #76]	@ 0x4c
	ldr	r0, [r3, #72]	@ 0x48
	str	r0, [r2, #72]	@ 0x48
	ldr	r2, [r3, #72]	@ 0x48
	ldr	r0, [r3, #76]	@ 0x4c
	str	r0, [r2, #76]	@ 0x4c
	movs	r0, #1
	ands	r0, r1
_0803B600: .4byte 0xd0022800
	adds	r0, r3, #0
	bl mem_free
_0803B60A:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B610: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B614
sub_803B614: @ 0x0803B614
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B63C
	str	r0, [r3, #80]	@ 0x50
	ldr	r2, [r3, #76]	@ 0x4c
	ldr	r0, [r3, #72]	@ 0x48
	str	r0, [r2, #72]	@ 0x48
	ldr	r2, [r3, #72]	@ 0x48
	ldr	r0, [r3, #76]	@ 0x4c
	str	r0, [r2, #76]	@ 0x4c
	movs	r0, #1
	ands	r0, r1
_0803B62C: .4byte 0xd0022800
	adds	r0, r3, #0
	bl mem_free
_0803B636:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B63C: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B640
sub_803B640: @ 0x0803B640
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B668
	str	r0, [r3, #80]	@ 0x50
	ldr	r2, [r3, #76]	@ 0x4c
	ldr	r0, [r3, #72]	@ 0x48
	str	r0, [r2, #72]	@ 0x48
	ldr	r2, [r3, #72]	@ 0x48
	ldr	r0, [r3, #76]	@ 0x4c
	str	r0, [r2, #76]	@ 0x4c
	movs	r0, #1
	ands	r0, r1
_0803B658: .4byte 0xd0022800
	adds	r0, r3, #0
	bl mem_free
_0803B662:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B668: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B66C
sub_803B66C: @ 0x0803B66C
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B694
	str	r0, [r3, #80]	@ 0x50
	ldr	r2, [r3, #76]	@ 0x4c
	ldr	r0, [r3, #72]	@ 0x48
	str	r0, [r2, #72]	@ 0x48
	ldr	r2, [r3, #72]	@ 0x48
	ldr	r0, [r3, #76]	@ 0x4c
	str	r0, [r2, #76]	@ 0x4c
	movs	r0, #1
	ands	r0, r1
_0803B684: .4byte 0xd0022800
	adds	r0, r3, #0
	bl mem_free
_0803B68E:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B694: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B698
sub_803B698: @ 0x0803B698
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B6C0
	str	r0, [r3, #80]	@ 0x50
	ldr	r2, [r3, #76]	@ 0x4c
	ldr	r0, [r3, #72]	@ 0x48
	str	r0, [r2, #72]	@ 0x48
	ldr	r2, [r3, #72]	@ 0x48
	ldr	r0, [r3, #76]	@ 0x4c
	str	r0, [r2, #76]	@ 0x4c
	movs	r0, #1
	ands	r0, r1
_0803B6B0: .4byte 0xd0022800
	adds	r0, r3, #0
	bl mem_free
_0803B6BA:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B6C0: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B6C4
sub_803B6C4: @ 0x0803B6C4
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B6EC
	str	r0, [r3, #80]	@ 0x50
	ldr	r2, [r3, #76]	@ 0x4c
	ldr	r0, [r3, #72]	@ 0x48
	str	r0, [r2, #72]	@ 0x48
	ldr	r2, [r3, #72]	@ 0x48
	ldr	r0, [r3, #76]	@ 0x4c
	str	r0, [r2, #76]	@ 0x4c
	movs	r0, #1
	ands	r0, r1
_0803B6DC: .4byte 0xd0022800
	adds	r0, r3, #0
	bl mem_free
_0803B6E6:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B6EC: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B6F0
sub_803B6F0: @ 0x0803B6F0
	push	{r4, r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	movs	r1, #0
	bl sub_80321D0
	movs	r0, #1
	ands	r0, r4
	cmp	r0, #0
	beq _0803B70A
	adds	r0, r5, #0
	bl mem_free
_0803B70A:
	pop	{r4, r5}
	pop	{r0}
	bx	r0

	thumb_func_start sub_803B710
sub_803B710: @ 0x0803B710
	push	{r4, r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	movs	r1, #0
	bl sub_80321D0
	movs	r0, #1
	ands	r0, r4
	cmp	r0, #0
	beq _0803B72A
	adds	r0, r5, #0
	bl mem_free
_0803B72A:
	pop	{r4, r5}
	pop	{r0}
	bx	r0

	thumb_func_start sub_803B730
sub_803B730: @ 0x0803B730
	push	{r4, r5, lr}
	adds	r5, r0, #0
_0803B734: .4byte 0x21001c0c
	bl sub_80321D0
	movs	r0, #1
	ands	r0, r4
	cmp	r0, #0
	beq _0803B74A
	adds	r0, r5, #0
	bl mem_free
_0803B74A:
	pop	{r4, r5}
	pop	{r0}
	bx	r0

	thumb_func_start sub_803B750
sub_803B750: @ 0x0803B750
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B778
	str	r0, [r3, #80]	@ 0x50
	ldr	r2, [r3, #76]	@ 0x4c
	ldr	r0, [r3, #72]	@ 0x48
	str	r0, [r2, #72]	@ 0x48
	ldr	r2, [r3, #72]	@ 0x48
_0803B760: .4byte 0x64d06cd8
	movs	r0, #1
	ands	r0, r1
	cmp	r0, #0
	beq _0803B772
	adds	r0, r3, #0
	bl mem_free
_0803B772:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B778: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B77C
sub_803B77C: @ 0x0803B77C
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B7A4
	str	r0, [r3, #80]	@ 0x50
	ldr	r2, [r3, #76]	@ 0x4c
	ldr	r0, [r3, #72]	@ 0x48
	str	r0, [r2, #72]	@ 0x48
	ldr	r2, [r3, #72]	@ 0x48
_0803B78C: .4byte 0x64d06cd8
	movs	r0, #1
	ands	r0, r1
	cmp	r0, #0
	beq _0803B79E
	adds	r0, r3, #0
	bl mem_free
_0803B79E:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B7A4: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B7A8
sub_803B7A8: @ 0x0803B7A8
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B7D0
	str	r0, [r3, #80]	@ 0x50
	ldr	r2, [r3, #76]	@ 0x4c
	ldr	r0, [r3, #72]	@ 0x48
	str	r0, [r2, #72]	@ 0x48
	ldr	r2, [r3, #72]	@ 0x48
_0803B7B8: .4byte 0x64d06cd8
	movs	r0, #1
	ands	r0, r1
	cmp	r0, #0
	beq _0803B7CA
	adds	r0, r3, #0
	bl mem_free
_0803B7CA:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B7D0: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B7D4
sub_803B7D4: @ 0x0803B7D4
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B7FC
	str	r0, [r3, #80]	@ 0x50
	ldr	r2, [r3, #76]	@ 0x4c
	ldr	r0, [r3, #72]	@ 0x48
	str	r0, [r2, #72]	@ 0x48
	ldr	r2, [r3, #72]	@ 0x48
_0803B7E4: .4byte 0x64d06cd8
	movs	r0, #1
	ands	r0, r1
	cmp	r0, #0
	beq _0803B7F6
	adds	r0, r3, #0
	bl mem_free
_0803B7F6:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B7FC: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B800
sub_803B800: @ 0x0803B800
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B828
	str	r0, [r3, #80]	@ 0x50
	ldr	r2, [r3, #76]	@ 0x4c
	ldr	r0, [r3, #72]	@ 0x48
	str	r0, [r2, #72]	@ 0x48
	ldr	r2, [r3, #72]	@ 0x48
_0803B810: .4byte 0x64d06cd8
	movs	r0, #1
	ands	r0, r1
	cmp	r0, #0
	beq _0803B822
	adds	r0, r3, #0
	bl mem_free
_0803B822:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B828: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B82C
sub_803B82C: @ 0x0803B82C
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B854
	str	r0, [r3, #80]	@ 0x50
	ldr	r2, [r3, #76]	@ 0x4c
	ldr	r0, [r3, #72]	@ 0x48
	str	r0, [r2, #72]	@ 0x48
	ldr	r2, [r3, #72]	@ 0x48
_0803B83C: .4byte 0x64d06cd8
	movs	r0, #1
	ands	r0, r1
	cmp	r0, #0
	beq _0803B84E
	adds	r0, r3, #0
	bl mem_free
_0803B84E:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B854: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B858
sub_803B858: @ 0x0803B858
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B880
	str	r0, [r3, #80]	@ 0x50
	ldr	r2, [r3, #76]	@ 0x4c
	ldr	r0, [r3, #72]	@ 0x48
	str	r0, [r2, #72]	@ 0x48
	ldr	r2, [r3, #72]	@ 0x48
	ldr	r0, [r3, #76]	@ 0x4c
	str	r0, [r2, #76]	@ 0x4c
	movs	r0, #1
	ands	r0, r1
	cmp	r0, #0
	beq _0803B87A
	adds	r0, r3, #0
	bl mem_free
_0803B87A:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B880: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B884
sub_803B884: @ 0x0803B884
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B8AC
	str	r0, [r3, #80]	@ 0x50
	ldr	r2, [r3, #76]	@ 0x4c
	ldr	r0, [r3, #72]	@ 0x48
	str	r0, [r2, #72]	@ 0x48
	ldr	r2, [r3, #72]	@ 0x48
	ldr	r0, [r3, #76]	@ 0x4c
	str	r0, [r2, #76]	@ 0x4c
	movs	r0, #1
	ands	r0, r1
	cmp	r0, #0
	beq _0803B8A6
	adds	r0, r3, #0
	bl mem_free
_0803B8A6:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B8AC: .4byte gStaticData_087E4DF4
