.include "asm/macros.inc"

.syntax unified
.arm
	thumb_func_start sub_8031378
sub_8031378: @ 0x08031378
	push {r4, r5, lr}
	sub sp, #0x24
	adds r5, r0, #0
	ldr r0, _0803144C @ =gUnknown_03001538
	ldr r0, [r0]
	subs r0, #2
	cmp r0, #1
	bhi _08031448
	mov r1, sp
	ldr r0, _08031450 @ =gStaticData_0817C3D8
	ldm r0!, {r2, r3, r4}
	stm r1!, {r2, r3, r4}
	ldr r0, _08031454 @ =gUnknown_03001540
	ldr r3, [r0]
	asrs r3, r3, #8
	ldr r0, _08031458 @ =gUnknown_03001544
	ldr r4, [r0]
	asrs r4, r4, #8
	ldr r0, _0803145C @ =gUnknown_03001548
	ldr r2, [r0]
	asrs r2, r2, #8
	mov r1, sp
	ldrh r0, [r1]
	adds r3, r0, r3
	strh r3, [r1]
	ldrh r0, [r1, #2]
	adds r0, r0, r4
	strh r0, [r1, #2]
	ldrh r3, [r1, #4]
	adds r2, r3, r2
	strh r2, [r1, #4]
	add r1, sp, #0x18
	adds r0, r5, #0
	adds r0, #0x38
	ldm r0!, {r2, r3, r4}
	stm r1!, {r2, r3, r4}
	ldr r0, [r5, #0x1c]
	asrs r0, r0, #8
	ldr r3, [r5, #0x20]
	asrs r3, r3, #8
	ldr r2, [r5, #0x24]
	asrs r2, r2, #8
	add r1, sp, #0x18
	ldrh r4, [r1]
	adds r0, r4, r0
	strh r0, [r1]
	ldrh r0, [r1, #2]
	adds r0, r0, r3
	strh r0, [r1, #2]
	ldrh r5, [r1, #4]
	adds r2, r5, r2
	strh r2, [r1, #4]
	add r0, sp, #0xc
	ldm r1!, {r2, r3, r4}
	stm r0!, {r2, r3, r4}
	add r4, sp, #0xc
	adds r0, r4, #0
	adds r1, r4, #0
	movs r2, #0xc
	bl sub_800014C
	mov r1, sp
	movs r5, #4
	ldrsh r2, [r1, r5]
	movs r0, #4
	ldrsh r3, [r4, r0]
	movs r5, #0xa
	ldrsh r0, [r4, r5]
	adds r0, r3, r0
	cmp r2, r0
	bge _08031448
	movs r5, #0xa
	ldrsh r0, [r1, r5]
	adds r0, r2, r0
	cmp r0, r3
	ble _08031448
	movs r0, #2
	ldrsh r2, [r1, r0]
	movs r5, #2
	ldrsh r3, [r4, r5]
	movs r5, #8
	ldrsh r0, [r4, r5]
	adds r0, r3, r0
	cmp r2, r0
	bge _08031448
	movs r5, #8
	ldrsh r0, [r1, r5]
	adds r0, r2, r0
	cmp r0, r3
	ble _08031448
	movs r0, #0
	ldrsh r2, [r1, r0]
	movs r5, #0
	ldrsh r3, [r4, r5]
	movs r5, #6
	ldrsh r0, [r4, r5]
	adds r0, r3, r0
	cmp r2, r0
	bge _08031448
	movs r4, #6
	ldrsh r0, [r1, r4]
	adds r0, r2, r0
	cmp r0, r3
	bgt _08031460
_08031448:
	movs r0, #0
	b _08031462
	.align 2, 0
_0803144C: .4byte gUnknown_03001538
_08031450: .4byte gStaticData_0817C3D8
_08031454: .4byte gUnknown_03001540
_08031458: .4byte gUnknown_03001544
_0803145C: .4byte gUnknown_03001548
_08031460:
	movs r0, #1
_08031462:
	add sp, #0x24
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0

