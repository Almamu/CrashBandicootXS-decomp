.include "asm/macros.inc"

.syntax unified
.arm

.if NON_MATCHING == 0
	thumb_func_start sub_80057E0
sub_80057E0: @ 0x080057E0
	push {r4, r5, r6, lr}
	adds r5, r0, #0
	ldr r1, [r5, #0x10]
	movs r0, #1
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _080057FE
	adds r0, r5, #0
	adds r0, #0xa0
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
_080057FE:
	ldr r1, [r5, #0x10]
	movs r0, #4
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _08005818
	adds r0, r5, #0
	adds r0, #0xa4
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
_08005818:
	ldr r1, [r5, #0x10]
	movs r0, #8
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _08005832
	adds r0, r5, #0
	adds r0, #0xa8
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
_08005832:
	ldr r1, [r5, #0x10]
	movs r0, #2
	ldrb r1, [r1, #2]
	ands r0, r1
	cmp r0, #0
	beq _0800584C
	adds r0, r5, #0
	adds r0, #0xac
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
_0800584C:
	adds r0, r5, #0
	adds r0, #0x9c
	ldr r0, [r0]
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	ldr r0, _080058B8 @ =gStaticData_0816B21C
	ldr r4, _080058BC @ =gUnknown_030012DC
	ldr r3, [r4]
	ldr r1, [r0]
	subs r1, #0x14
	ldr r2, [r0, #4]
	subs r2, #4
	movs r6, #0x88
	lsls r6, r6, #1
	adds r0, r3, r6
	str r1, [r0]
	movs r1, #0x8a
	lsls r1, r1, #1
	adds r0, r3, r1
	str r2, [r0]
	adds r6, #0x20
	adds r0, r3, r6
	ldr r2, [r0]
	movs r1, #0x20
	ldrsh r0, [r2, r1]
	adds r0, r3, r0
	adds r1, r5, #0
	adds r1, #0x2f
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r3, [r4]
	movs r1, #0xb4
	movs r2, #0x80
	movs r4, #0x88
	lsls r4, r4, #1
	adds r0, r3, r4
	str r1, [r0]
	subs r6, #0x1c
	adds r0, r3, r6
	str r2, [r0]
	adds r1, r5, #0
	adds r1, #0x32
	adds r2, r5, #0
	adds r2, #0x49
	adds r0, r5, #0
	bl sub_8005E5C
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_080058B8: .4byte gStaticData_0816B21C
_080058BC: .4byte gUnknown_030012DC
.endif

.if NON_MATCHING == 0
	thumb_func_start sub_80058C0
sub_80058C0: @ 0x080058C0
	push {r4, r5, r6, r7, lr}
	adds r7, r0, #0
	adds r5, r7, #0
	adds r5, #0xb0
	movs r4, #2
_080058CA:
	ldm r5!, {r0}
	movs r1, #0
	movs r2, #0
	bl sub_8008890
	subs r4, #1
	cmp r4, #0
	bge _080058CA
	ldr r4, _08005994 @ =gStaticData_0816B258
	ldr r6, _08005998 @ =gUnknown_030012DC
	ldr r0, [r6]
	ldr r2, [r4, #0x10]
	subs r2, #4
	ldr r3, [r4, #0x14]
	adds r3, #0xe
	movs r5, #0x88
	lsls r5, r5, #1
	adds r1, r0, r5
	str r2, [r1]
	movs r2, #0x8a
	lsls r2, r2, #1
	adds r1, r0, r2
	str r3, [r1]
	adds r5, #0x20
	adds r1, r0, r5
	ldr r2, [r1]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	adds r1, r7, #0
	adds r1, #0x38
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r0, [r6]
	ldr r2, [r4, #8]
	subs r2, #4
	ldr r3, [r4, #0xc]
	adds r3, #0xe
	mov ip, r3
	movs r3, #0x88
	lsls r3, r3, #1
	adds r1, r0, r3
	str r2, [r1]
	movs r2, #0x8a
	lsls r2, r2, #1
	adds r1, r0, r2
	mov r3, ip
	str r3, [r1]
	adds r1, r0, r5
	ldr r2, [r1]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	adds r1, r7, #0
	adds r1, #0x3b
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r0, [r6]
	ldr r2, [r4]
	subs r2, #4
	ldr r3, [r4, #4]
	adds r3, #0xe
	movs r4, #0x88
	lsls r4, r4, #1
	adds r1, r0, r4
	str r2, [r1]
	movs r2, #0x8a
	lsls r2, r2, #1
	adds r1, r0, r2
	str r3, [r1]
	adds r5, r0, r5
	ldr r2, [r5]
	movs r3, #0x20
	ldrsh r1, [r2, r3]
	adds r0, r0, r1
	adds r1, r7, #0
	adds r1, #0x3e
	ldr r2, [r2, #0x24]
	bl sub_803AD80
	ldr r3, [r6]
	movs r1, #0xb4
	movs r2, #0x80
	adds r0, r3, r4
	str r1, [r0]
	adds r4, #4
	adds r0, r3, r4
	str r2, [r0]
	adds r1, r7, #0
	adds r1, #0x35
	adds r2, r7, #0
	adds r2, #0x4c
	adds r0, r7, #0
	bl sub_8005E5C
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08005994: .4byte gStaticData_0816B258
_08005998: .4byte gUnknown_030012DC
.endif
