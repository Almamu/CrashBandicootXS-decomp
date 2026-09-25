.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_800C18C
sub_800C18C: @ 0x0800C18C
	push {r4, lr}
	adds r3, r0, #0
	ldr r2, [r3, #0x70]
	ldr r4, [r2]
	ldr r0, _0800C1AC @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0]
	subs r1, r4, r0
	cmp r1, #0x14
	ble _0800C1BE
	ldr r0, [r3, #0x10]
	cmp r4, r0
	bge _0800C1B0
	movs r1, #0
	movs r0, #0x10
	b _0800C1DA
	.align 2, 0
_0800C1AC: .4byte gUnknown_030012D8
_0800C1B0:
	ldr r0, [r3, #0x58]
	rsbs r0, r0, #0
	ldr r1, [r3, #0x5c]
	str r0, [r2, #0x48]
	str r1, [r2, #0x4c]
	str r0, [r2, #0x50]
	b _0800C1E0
_0800C1BE:
	movs r0, #0x14
	rsbs r0, r0, #0
	cmp r1, r0
	bge _0800C1D6
	ldr r0, [r3, #0x14]
	cmp r4, r0
	ble _0800C1D2
	movs r1, #0
	movs r0, #0x10
	b _0800C1DA
_0800C1D2:
	ldr r1, [r3, #0x58]
	b _0800C1D8
_0800C1D6:
	movs r1, #0
_0800C1D8:
	ldr r0, [r3, #0x5c]
_0800C1DA:
	str r1, [r2, #0x48]
	str r0, [r2, #0x4c]
	str r1, [r2, #0x50]
_0800C1E0:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0

	thumb_func_start sub_800C1E8
sub_800C1E8: @ 0x0800C1E8
	push {r4, lr}
	adds r3, r0, #0
	ldr r2, [r3, #0x70]
	ldr r4, [r2, #4]
	ldr r0, _0800C208 @ =gUnknown_030012D8
	ldr r0, [r0]
	ldr r0, [r0, #4]
	subs r1, r4, r0
	cmp r1, #0x14
	ble _0800C21A
	ldr r0, [r3, #0x1c]
	cmp r4, r0
	bge _0800C20C
	movs r1, #0
	movs r0, #0x10
	b _0800C236
	.align 2, 0
_0800C208: .4byte gUnknown_030012D8
_0800C20C:
	ldr r0, [r3, #0x58]
	rsbs r0, r0, #0
	ldr r1, [r3, #0x5c]
	str r0, [r2, #0x54]
	str r1, [r2, #0x58]
	str r0, [r2, #0x5c]
	b _0800C23C
_0800C21A:
	movs r0, #0x14
	rsbs r0, r0, #0
	cmp r1, r0
	bge _0800C232
	ldr r0, [r3, #0x18]
	cmp r4, r0
	ble _0800C22E
	movs r1, #0
	movs r0, #0x10
	b _0800C236
_0800C22E:
	ldr r1, [r3, #0x58]
	b _0800C234
_0800C232:
	movs r1, #0
_0800C234:
	ldr r0, [r3, #0x5c]
_0800C236:
	str r1, [r2, #0x54]
	str r0, [r2, #0x58]
	str r1, [r2, #0x5c]
_0800C23C:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
