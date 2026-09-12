.include "asm/macros.inc"

.syntax unified
.arm
	thumb_func_start sub_803B060
sub_803B060: @ 0x0803B060
	ldr r1, [r0, #0xc]
	ldr r2, [r0]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #2
	adds r0, r0, r2
	ldrh r0, [r0, #8]
	lsls r0, r0, #0x10
	bx lr
	.align 2, 0

	thumb_func_start GetAnimFrameData
GetAnimFrameData: @ 0x0803B074
	push {r4, r5, lr}
	adds r4, r0, #0
	bl GetAnimFrameBaseOffset
	ldr r5, _0803B0A4 @ =gUnknown_0300137C
	ldr r2, [r4, #0xc]
	ldr r3, [r4]
	lsls r1, r2, #1
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r1, r1, r3
	movs r2, #2
	ldrsh r1, [r1, r2]
	adds r1, r1, r0
	ldr r0, [r4, #4]
	lsls r1, r1, #2
	adds r1, r1, r0
	ldr r0, [r5]
	ldr r1, [r1]
	adds r0, r0, r1
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0803B0A4: .4byte gUnknown_0300137C

	thumb_func_start sub_803B0A8
sub_803B0A8: @ 0x0803B0A8
	str r1, [r0, #0xc]
	ldr r3, [r0]
	lsls r2, r1, #1
	adds r2, r2, r1
	lsls r2, r2, #2
	adds r2, r2, r3
	ldrh r1, [r2]
	movs r2, #0
	movs r3, #0
	strh r1, [r0, #0x10]
	strb r2, [r0, #0x12]
	str r3, [r0, #8]
	bx lr
	.align 2, 0

	thumb_func_start sub_803B0C4
sub_803B0C4: @ 0x0803B0C4
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B0EC
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
	beq _0803B0E6
	adds	r0, r3, #0
	bl mem_free
_0803B0E6:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B0EC: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B0F0
sub_803B0F0: @ 0x0803B0F0
	push	{lr}
	adds	r2, r0, #0
	ldr	r0, [r2, #32]
	ldr r1, _0803B118
	adds	r0, r0, r1
	str	r0, [r2, #32]
	ldrb	r0, [r2, #18]
	cmp	r0, #0
	beq _0803B11C
	cmp	r2, #0
	beq _0803B122
	ldr	r1, [r2, #80]	@ 0x50
	movs	r3, #8
	ldrsh	r0, [r1, r3]
	adds	r0, r2, r0
	ldr	r2, [r1, #12]
	movs	r1, #3
	bl sub_803AD80
	b _0803B122
_0803B118: .4byte 0xfffffe80
_0803B11C:
	adds	r0, r2, #0
	bl sub_802A7B8
_0803B122:
	pop	{r0}
	bx	r0
	movs	r0, r0

	thumb_func_start sub_803B128
sub_803B128: @ 0x0803B128
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B150
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
	beq _0803B14A
	adds	r0, r3, #0
	bl mem_free
_0803B14A:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B150: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B154
sub_803B154: @ 0x0803B154
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B17C
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
	beq _0803B176
	adds	r0, r3, #0
	bl mem_free
_0803B176:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B17C: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B180
sub_803B180: @ 0x0803B180
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B1A8
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
	beq _0803B1A2
	adds	r0, r3, #0
	bl mem_free
_0803B1A2:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B1A8: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B1AC
sub_803B1AC: @ 0x0803B1AC
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B1D4
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
	beq _0803B1CE
	adds	r0, r3, #0
	bl mem_free
_0803B1CE:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B1D4: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B1D8
sub_803B1D8: @ 0x0803B1D8
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B200
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
	beq _0803B1FA
	adds	r0, r3, #0
	bl mem_free
_0803B1FA:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B200: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B204
sub_803B204: @ 0x0803B204
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B22C
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
	beq _0803B226
	adds	r0, r3, #0
	bl mem_free
_0803B226:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B22C: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B230
sub_803B230: @ 0x0803B230
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B258
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
	beq _0803B252
	adds	r0, r3, #0
	bl mem_free
_0803B252:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B258: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B25C
sub_803B25C: @ 0x0803B25C
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B284
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
	beq _0803B27E
	adds	r0, r3, #0
	bl mem_free
_0803B27E:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B284: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B288
sub_803B288: @ 0x0803B288
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B2B0
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
	beq _0803B2AA
	adds	r0, r3, #0
	bl mem_free
_0803B2AA:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B2B0: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B2B4
sub_803B2B4: @ 0x0803B2B4
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B2DC
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
	beq _0803B2D6
	adds	r0, r3, #0
	bl mem_free
_0803B2D6:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B2DC: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B2E0
sub_803B2E0: @ 0x0803B2E0
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B308
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
	beq _0803B302
	adds	r0, r3, #0
	bl mem_free
_0803B302:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B308: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B30C
sub_803B30C: @ 0x0803B30C
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B334
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
	beq _0803B32E
	adds	r0, r3, #0
	bl mem_free
_0803B32E:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B334: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B338
sub_803B338: @ 0x0803B338
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B360
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
	beq _0803B35A
	adds	r0, r3, #0
	bl mem_free
_0803B35A:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B360: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B364
sub_803B364: @ 0x0803B364
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B38C
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
	beq _0803B386
	adds	r0, r3, #0
	bl mem_free
_0803B386:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B38C: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B390
sub_803B390: @ 0x0803B390
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B3B8
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
	beq _0803B3B2
	adds	r0, r3, #0
	bl mem_free
_0803B3B2:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B3B8: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B3BC
sub_803B3BC: @ 0x0803B3BC
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B3E4
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
	beq _0803B3DE
	adds	r0, r3, #0
	bl mem_free
_0803B3DE:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B3E4: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B3E8
sub_803B3E8: @ 0x0803B3E8
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B410
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
	beq _0803B40A
	adds	r0, r3, #0
	bl mem_free
_0803B40A:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B410: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B414
sub_803B414: @ 0x0803B414
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B43C
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
	beq _0803B436
	adds	r0, r3, #0
	bl mem_free
_0803B436:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B43C: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B440
sub_803B440: @ 0x0803B440
	push	{lr}
	adds	r3, r0, #0
	ldr r0, _0803B468
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
	beq _0803B462
	adds	r0, r3, #0
	bl mem_free
_0803B462:
	pop	{r0}
	bx	r0
	movs	r0, r0
_0803B468: .4byte gStaticData_087E4DF4

	thumb_func_start sub_803B46C
sub_803B46C: @ 0x0803B46C
	push	{r4, r5, r6, r7, lr}
	adds	r5, r0, #0
	movs	r4, #120	@ 0x78
	movs	r6, #106	@ 0x6a
	bl GetAnimFrameData
	adds	r7, r0, #0
	ldrb	r0, [r7, #0]
	lsls	r2, r0, #2
	ldrb	r1, [r7, #1]
	lsls	r0, r1, #2
	subs	r4, r4, r2
	subs	r6, r6, r0
	cmp	r6, #159	@ 0x9f
	bgt _0803B4E4
	lsls	r0, r1, #3
	adds	r0, r6, r0
	cmp	r0, #0
	blt _0803B4E4
	cmp	r4, #239	@ 0xef
	bgt _0803B4E4
	lsls	r0, r2, #1
	adds	r0, r4, r0
	cmp	r0, #0
	blt _0803B4E4
	adds	r0, r5, #0
	bl sub_803B060
	movs	r3, #255	@ 0xff
	ands	r3, r6
	ldr r1, _0803B4D0
	ands	r4, r1
	lsls	r1, r4, #16
	orrs	r3, r1
	orrs	r3, r0
	movs	r0, #0
	orrs	r3, r0
	ldr	r4, [r5, #24]
	lsls	r2, r4, #12
	ldr	r0, [r5, #20]
	movs	r1, #128	@ 0x80
	lsls	r1, r1, #8
_0803B4C0: .4byte 0x28004008
	beq _0803B4D4
	movs	r0, #128	@ 0x80
	lsls	r0, r0, #4
	orrs	r2, r0
	lsls	r0, r2, #16
	b _0803B4D6
_0803B4D0: .4byte 0x1ff
_0803B4D4:
	lsls	r0, r4, #28
_0803B4D6:
	lsrs	r2, r0, #16
	adds	r0, r7, #0
	adds	r1, r3, #0
	movs	r3, #128	@ 0x80
	lsls	r3, r3, #1
	bl SetupSpriteFrameOam
_0803B4E4:
	pop	{r4, r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0

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
