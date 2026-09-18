.include "asm/macros.inc"

.syntax unified
.arm

@ sub_800A884 is reconstructed (but not yet byte-matching) as C in
@ src/graphics/actor_part78.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-9-10-0x0800a884-graphics.md.
.if NON_MATCHING == 0
	thumb_func_start sub_800A884
sub_800A884: @ 0x0800A884
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	ldrb r1, [r5, #0xc]
	lsrs r0, r1, #7
	cmp r0, #0
	bne _0800A892
	b _0800AADC
_0800A892:
	movs r4, #0
	adds r7, r5, #0
	adds r7, #0x68
	strb r4, [r7]
	ldr r2, _0800A90C @ =0x00000105
	adds r6, r5, r2
	strb r4, [r6]
	ldr r1, [r5, #0x18]
	adds r1, #0x70
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r1, [r1, #4]
	bl sub_803AD7C
	movs r1, #1
	strb r1, [r6]
	ldr r6, _0800A910 @ =gUnknown_03001308
	ldr r0, [r6]
	adds r0, #0x2a
	strb r1, [r0]
	adds r0, r5, #0
	bl sub_800A0FC
	ldr r0, [r6]
	adds r0, #0x2a
	strb r4, [r0]
	adds r1, r5, #0
	adds r1, #0xac
	ldr r0, [r1]
	cmp r0, #0
	beq _0800A8F2
	movs r0, #8
	ldrb r2, [r7]
	orrs r0, r2
	strb r0, [r7]
	str r4, [r1]
	movs r1, #0x80
	lsls r1, r1, #1
	adds r0, r5, r1
	strb r4, [r0]
	movs r2, #0x81
	lsls r2, r2, #1
	adds r0, r5, r2
	strb r4, [r0]
	adds r1, #3
	adds r0, r5, r1
	strb r4, [r0]
_0800A8F2:
	ldr r0, [r6]
	adds r0, #0x29
	ldrb r1, [r0]
	cmp r1, #0
	beq _0800A9DC
	subs r0, r1, #1
	cmp r0, #9
	bhi _0800A9CE
	lsls r0, r0, #2
	ldr r1, _0800A914 @ =_0800A918
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800A90C: .4byte 0x00000105
_0800A910: .4byte gUnknown_03001308
_0800A914: .4byte _0800A918
_0800A918: @ jump table
	.4byte _0800A940 @ case 0
	.4byte _0800A9CE @ case 1
	.4byte _0800A9CE @ case 2
	.4byte _0800A9CE @ case 3
	.4byte _0800A978 @ case 4
	.4byte _0800A9CE @ case 5
	.4byte _0800A998 @ case 6
	.4byte _0800A9CE @ case 7
	.4byte _0800A9CE @ case 8
	.4byte _0800A9B4 @ case 9
_0800A940:
	movs r0, #0x40
	ldrb r2, [r5, #0xc]
	orrs r0, r2
	strb r0, [r5, #0xc]
	adds r1, r5, #0
	adds r1, #0x8c
	movs r0, #0
	str r0, [r1]
	ldr r0, _0800A974 @ =gUnknown_030012C0
	ldr r0, [r0]
	movs r1, #0
	bl sub_80231EC
	ldr r1, [r5, #0x18]
	adds r1, #0x68
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #0
	bl sub_803AD88
	b _0800A9CE
	.align 2, 0
_0800A974: .4byte gUnknown_030012C0
_0800A978:
	movs r0, #0x81
	lsls r0, r0, #1
	adds r1, r5, r0
	movs r0, #0
	strb r0, [r1]
	ldr r2, _0800A994 @ =0x00000103
	adds r1, r5, r2
	strb r0, [r1]
	movs r1, #1
	subs r2, #3
	adds r0, r5, r2
	strb r1, [r0]
	b _0800A9CE
	.align 2, 0
_0800A994: .4byte 0x00000103
_0800A998:
	ldr r0, _0800A9B0 @ =0x00000103
	adds r1, r5, r0
	movs r0, #0
	strb r0, [r1]
	movs r2, #0x80
	lsls r2, r2, #1
	adds r1, r5, r2
	strb r0, [r1]
	movs r0, #1
	adds r2, #2
	b _0800A9CA
	.align 2, 0
_0800A9B0: .4byte 0x00000103
_0800A9B4:
	movs r0, #0x81
	lsls r0, r0, #1
	adds r1, r5, r0
	movs r0, #0
	strb r0, [r1]
	movs r2, #0x80
	lsls r2, r2, #1
	adds r1, r5, r2
	strb r0, [r1]
	movs r0, #1
	adds r2, #3
_0800A9CA:
	adds r1, r5, r2
	strb r0, [r1]
_0800A9CE:
	ldr r0, _0800A9D8 @ =gUnknown_03001308
	ldr r0, [r0]
	adds r0, #0x29
	movs r1, #0
	b _0800A9F4
	.align 2, 0
_0800A9D8: .4byte gUnknown_03001308
_0800A9DC:
	ldrb r7, [r7]
	cmp r7, #8
	bne _0800A9F6
	movs r2, #0x81
	lsls r2, r2, #1
	adds r0, r5, r2
	strb r1, [r0]
	adds r2, #1
	adds r0, r5, r2
	strb r1, [r0]
	subs r2, #3
	adds r0, r5, r2
_0800A9F4:
	strb r1, [r0]
_0800A9F6:
	adds r0, r5, #0
	bl sub_80083B8
	adds r2, r0, #0
	ldr r0, [r2, #4]
	ldrb r0, [r0]
	lsrs r0, r0, #4
	cmp r0, #6
	bhi _0800AA40
	lsls r0, r0, #2
	ldr r1, _0800AA14 @ =_0800AA18
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0800AA14: .4byte _0800AA18
_0800AA18: @ jump table
	.4byte _0800AA34 @ case 0
	.4byte _0800AA40 @ case 1
	.4byte _0800AA40 @ case 2
	.4byte _0800AA40 @ case 3
	.4byte _0800AA40 @ case 4
	.4byte _0800AA40 @ case 5
	.4byte _0800AA3A @ case 6
_0800AA34:
	adds r3, r2, #0
	adds r3, #0x24
	b _0800AA42
_0800AA3A:
	adds r3, r2, #0
	adds r3, #0x14
	b _0800AA42
_0800AA40:
	ldr r3, _0800AA60 @ =gStaticData_0816B300
_0800AA42:
	ldr r0, [r5]
	asrs r1, r0, #8
	ldr r0, [r5, #4]
	asrs r4, r0, #8
	adds r0, r5, #0
	adds r0, #0x28
	ldrb r0, [r0]
	lsls r0, r0, #0x1b
	cmp r0, #0
	bge _0800AA64
	movs r2, #0
	ldrsh r0, [r3, r2]
	subs r1, r1, r0
	b _0800AA6A
	.align 2, 0
_0800AA60: .4byte gStaticData_0816B300
_0800AA64:
	movs r2, #0
	ldrsh r0, [r3, r2]
	adds r1, r1, r0
_0800AA6A:
	movs r2, #2
	ldrsh r0, [r3, r2]
	adds r4, r4, r0
	ldr r0, _0800AAB0 @ =gUnknown_03001308
	ldr r0, [r0]
	adds r2, r4, #0
	bl sub_8026BC0
	cmp r0, #6
	bne _0800AABC
	ldr r1, _0800AAB4 @ =0x00000101
	adds r0, r5, r1
	ldrb r0, [r0]
	cmp r0, #0
	bne _0800AADC
	ldr r0, _0800AAB8 @ =0x00FFFFF8
	ands r0, r4
	adds r0, #7
	subs r0, r0, r4
	lsls r0, r0, #8
	ldr r1, [r5, #4]
	adds r1, r1, r0
	str r1, [r5, #4]
	ldr r1, [r5, #0x18]
	adds r1, #0x68
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0x17
	movs r3, #0
	bl sub_803AD88
	b _0800AADC
	.align 2, 0
_0800AAB0: .4byte gUnknown_03001308
_0800AAB4: .4byte 0x00000101
_0800AAB8: .4byte 0x00FFFFF8
_0800AABC:
	ldr r1, _0800AAE8 @ =0x00000101
	adds r0, r5, r1
	ldrb r0, [r0]
	cmp r0, #0
	beq _0800AADC
	ldr r1, [r5, #0x18]
	adds r1, #0x68
	movs r2, #0
	ldrsh r0, [r1, r2]
	adds r0, r5, r0
	ldr r4, [r1, #4]
	movs r1, #0
	movs r2, #0x18
	movs r3, #0
	bl sub_803AD88
_0800AADC:
	adds r0, r5, #0
	adds r0, #0x68
	ldrb r0, [r0]
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0800AAE8: .4byte 0x00000101
.endif
