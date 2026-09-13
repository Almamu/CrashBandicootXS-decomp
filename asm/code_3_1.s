.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_80007EC
sub_80007EC: @ 0x080007EC
	push {r4, r5, r6, lr}
	mov r6, sb
	mov r5, r8
	push {r5, r6}
	mov sb, r0
	mov r8, r1
	ldr r1, _08000890 @ =0x0400000C
	ldr r2, _08000894 @ =0x0000088F
	adds r0, r2, #0
	strh r0, [r1]
	subs r1, #0xc
	ldr r2, _08000898 @ =0x00001F44
	adds r0, r2, #0
	strh r0, [r1]
	movs r5, #0x80
	lsls r5, r5, #1
	adds r0, r5, #0
	bl sub_800090C
	adds r4, r0, #0
	lsls r4, r4, #0x10
	lsrs r4, r4, #0x10
	movs r6, #0
	adds r0, r5, #0
	bl sub_800090C
	lsls r0, r0, #0x10
	lsls r1, r4, #0x10
	asrs r1, r1, #0x10
	lsls r2, r1, #4
	subs r2, r2, r1
	lsls r2, r2, #3
	ldr r3, _0800089C @ =0x00007788
	subs r3, r3, r2
	ldr r2, _080008A0 @ =0x00004FB0
	lsrs r5, r0, #0x10
	asrs r0, r0, #0x10
	lsls r1, r0, #2
	adds r1, r1, r0
	lsls r1, r1, #4
	subs r2, r2, r1
	ldr r0, _080008A4 @ =0x04000020
	strh r4, [r0]
	adds r0, #2
	strh r6, [r0]
	adds r0, #2
	strh r6, [r0]
	adds r0, #2
	strh r5, [r0]
	adds r0, #2
	strh r3, [r0]
	adds r0, #2
	ldr r1, _080008A8 @ =0x0FFF0000
	ands r3, r1
	asrs r3, r3, #0x10
	strh r3, [r0]
	adds r0, #2
	strh r2, [r0]
	adds r0, #2
	ands r2, r1
	asrs r2, r2, #0x10
	strh r2, [r0]
	ldr r1, _080008AC @ =0x040000D4
	mov r0, r8
	str r0, [r1]
	movs r0, #0xa0
	lsls r0, r0, #0x13
	str r0, [r1, #4]
	ldr r0, _080008B0 @ =0x80000100
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	movs r1, #0xc0
	lsls r1, r1, #0x13
	mov r0, sb
	bl LoadTaggedAsset
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08000890: .4byte 0x0400000C
_08000894: .4byte 0x0000088F
_08000898: .4byte 0x00001F44
_0800089C: .4byte 0x00007788
_080008A0: .4byte 0x00004FB0
_080008A4: .4byte 0x04000020
_080008A8: .4byte 0x0FFF0000
_080008AC: .4byte 0x040000D4
_080008B0: .4byte 0x80000100
