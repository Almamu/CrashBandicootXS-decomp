.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8022EA8/sub_8022F2C are reconstructed (but not yet byte-matching)
@ as C in src/system/game_loop2.c, guarded by #if NON_MATCHING - this
@ raw version is only assembled for the default (matching) build. See
@ docs/matching.md, GitHub issue #34's entry.
.if NON_MATCHING == 0
	thumb_func_start sub_8022EA8
sub_8022EA8: @ 0x08022EA8
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	adds r4, r1, #0
	ldr r0, _08022F20 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0x18
	bl PlaySfx
	adds r2, r5, #0
	adds r2, #0xa0
	lsls r1, r4, #4
	subs r1, r1, r4
	lsls r1, r1, #2
	ldr r0, [r2]
	adds r0, r0, r1
	str r0, [r2]
	ldr r7, _08022F24 @ =gUnknown_030012B8
	ldr r0, [r7]
	ldr r4, _08022F28 @ =gUnknown_030012D0
	ldr r1, [r4]
	ldr r1, [r1]
	ldr r1, [r1]
	movs r2, #0x8d
	lsls r2, r2, #2
	adds r1, r1, r2
	ldr r1, [r1]
	adds r1, #0x30
	ldrb r1, [r1]
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r6, r0, #0x18
	ldr r0, [r4]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x8d
	lsls r1, r1, #2
	adds r0, r0, r1
	ldr r0, [r0]
	adds r0, #0x84
	ldrb r2, [r0]
	ldr r0, [r7]
	adds r1, r6, #0
	bl sub_8006D08
	adds r5, #0xdc
	ldr r0, [r5]
	ldr r0, [r0, #8]
	cmp r0, #3
	bne _08022F18
	ldr r0, [r7]
	adds r1, r6, #0
	bl sub_8006DA0
_08022F18:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08022F20: .4byte gUnknown_030012BC
_08022F24: .4byte gUnknown_030012B8
_08022F28: .4byte gUnknown_030012D0

	thumb_func_start sub_8022F2C
sub_8022F2C: @ 0x08022F2C
	push {r4, r5, r6, r7, lr}
	adds r5, r0, #0
	adds r1, r5, #0
	adds r1, #0xa0
	ldr r3, [r1]
	cmp r3, #0
	beq _08022F9C
	subs r0, r3, #1
	str r0, [r1]
	cmp r0, #0
	bne _08022FE4
	ldr r7, _08022F94 @ =gUnknown_030012B8
	ldr r0, [r7]
	ldr r4, _08022F98 @ =gUnknown_030012D0
	ldr r1, [r4]
	ldr r1, [r1]
	ldr r1, [r1]
	movs r2, #0x8d
	lsls r2, r2, #2
	adds r1, r1, r2
	ldr r1, [r1]
	adds r1, #0x30
	ldrb r1, [r1]
	bl sub_8006DF8
	lsls r0, r0, #0x18
	lsrs r6, r0, #0x18
	ldr r0, [r4]
	ldr r0, [r0]
	ldr r0, [r0]
	movs r1, #0x8d
	lsls r1, r1, #2
	adds r0, r0, r1
	ldr r0, [r0]
	adds r0, #0x30
	ldrb r2, [r0]
	ldr r0, [r7]
	adds r1, r6, #0
	bl sub_8006D08
	adds r0, r5, #0
	adds r0, #0xdc
	ldr r0, [r0]
	ldr r0, [r0, #8]
	cmp r0, #3
	bne _08022FE4
	ldr r0, [r7]
	adds r1, r6, #0
	bl sub_8006DA0
	b _08022FE4
	.align 2, 0
_08022F94: .4byte gUnknown_030012B8
_08022F98: .4byte gUnknown_030012D0
_08022F9C:
	adds r1, r5, #0
	adds r1, #0x9c
	ldr r0, [r1]
	adds r6, r1, #0
	cmp r0, #5
	bne _08022FE0
	subs r1, #4
	ldr r0, [r1]
	adds r4, r1, #0
	cmp r0, #9
	bne _08022FD6
	subs r1, #4
	ldr r0, [r1]
	cmp r0, #0x3b
	bne _08022FCC
	adds r2, r5, #0
	adds r2, #0x90
	ldr r0, [r2]
	cmp r0, #0x63
	beq _08022FE4
	adds r0, #1
	str r0, [r2]
	str r3, [r1]
	b _08022FD0
_08022FCC:
	adds r0, #1
	str r0, [r1]
_08022FD0:
	movs r0, #0
	str r0, [r4]
	b _08022FDA
_08022FD6:
	adds r0, #1
	str r0, [r1]
_08022FDA:
	movs r0, #0
	str r0, [r6]
	b _08022FE4
_08022FE0:
	adds r0, #1
	str r0, [r1]
_08022FE4:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0

.endif
