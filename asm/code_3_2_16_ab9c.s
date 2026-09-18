.include "asm/macros.inc"

.syntax unified
.arm

@ sub_800AB9C is reconstructed (but not yet byte-matching) as C in
@ src/graphics/actor_part81.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-9-10-0x0800ab9c-graphics.md.
.if NON_MATCHING == 0
	thumb_func_start sub_800AB9C
sub_800AB9C: @ 0x0800AB9C
	push {r4, r5, r6, lr}
	sub sp, #0x2c
	adds r5, r0, #0
	ldr r1, _0800AC1C @ =0x00000105
	adds r0, r5, r1
	ldrb r6, [r0]
	cmp r6, #0
	bne _0800AC12
	ldrb r1, [r5, #0xc]
	lsrs r0, r1, #1
	movs r1, #1
	ands r0, r1
	cmp r0, #0
	beq _0800ABE8
	add r0, sp, #0xc
	adds r1, r5, #0
	bl sub_8007C30
	ldr r0, _0800AC20 @ =gUnknown_030012F0
	ldr r4, [r0]
	add r0, sp, #0x1c
	add r1, sp, #0xc
	movs r2, #0x10
	bl sub_800014C
	adds r0, r5, #0
	adds r0, #0x24
	ldrb r0, [r0]
	str r0, [sp, #4]
	str r5, [sp, #8]
	ldr r0, [sp, #0x28]
	str r0, [sp]
	ldr r1, [sp, #0x1c]
	ldr r2, [sp, #0x20]
	ldr r3, [sp, #0x24]
	adds r0, r4, #0
	bl sub_8008A40
_0800ABE8:
	ldrb r1, [r5, #0xc]
	lsrs r0, r1, #7
	cmp r0, #0
	beq _0800AC12
	movs r1, #0x84
	lsls r1, r1, #1
	adds r0, r5, r1
	str r6, [r0]
	strb r6, [r0, #4]
	ldr r0, _0800AC24 @ =gUnknown_0300130C
	ldr r0, [r0]
	movs r1, #3
	bl sub_8009868
	ldr r0, _0800AC28 @ =gUnknown_030012EC
	ldr r0, [r0]
	movs r1, #4
	bl sub_8008D30
	bl sub_80106DC
_0800AC12:
	add sp, #0x2c
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0800AC1C: .4byte 0x00000105
_0800AC20: .4byte gUnknown_030012F0
_0800AC24: .4byte gUnknown_0300130C
_0800AC28: .4byte gUnknown_030012EC
.endif
