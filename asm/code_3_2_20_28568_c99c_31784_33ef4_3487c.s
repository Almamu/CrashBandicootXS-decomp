.include "asm/macros.inc"

.syntax unified
.arm

@ sub_803487C: not yet matched (NON_MATCHING) - see
@ src/graphics/actor_part87.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. Fully
@ semantically understood and matches the ROM instruction-for-instruction
@ except the tile-cache seeding loop's trip counter, which the ROM keeps
@ live in r7 for the whole loop - this project's confirmed categorical
@ gcc-2.9 r7-pin bug (an explicit `register T x asm("r7")` compiles the
@ right instructions but never makes it into this compiler's own
@ push/pop-list computation) - see docs/matching/issue-63-0x08033ef4-actor.md
@ and docs/matching/issue-30-graphics-loading.md's LoadGraphicsPackage entry
@ for the same bug.
.if NON_MATCHING == 0
	thumb_func_start sub_803487C
sub_803487C: @ 0x0803487C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r0
	ldr r5, _08034974 @ =gUnknown_030012FC
	ldr r0, [r5]
	movs r4, #0
	str r4, [r0, #8]
	bl sub_8006C4C
	ldr r0, [r5]
	bl sub_8006C4C
	ldr r0, _08034978 @ =gUnknown_030012DC
	ldr r0, [r0]
	mov r1, r8
	str r0, [r1, #0x18]
	movs r2, #0x84
	lsls r2, r2, #1
	adds r1, r0, r2
	str r4, [r1]
	movs r3, #0x98
	lsls r3, r3, #1
	adds r1, r0, r3
	ldr r1, [r1]
	adds r1, #0x40
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r0, r0, r2
	ldr r1, [r1, #4]
	bl sub_803AD7C
	mov r0, r8
	ldr r1, [r0, #0x18]
	movs r2, #0x8c
	lsls r2, r2, #1
	adds r0, r1, r2
	str r4, [r0]
	ldr r0, [r5]
	movs r3, #0x96
	lsls r3, r3, #1
	adds r1, r1, r3
	ldr r1, [r1]
	lsls r1, r1, #5
	bl sub_8006C58
	ldr r0, [r5]
	bl sub_8006C30
	ldr r4, _0803497C @ =gUnknown_030012B8
	ldr r0, [r4]
	bl sub_8006EA8
	ldr r0, [r4]
	movs r1, #0
	bl sub_8006D50
	ldr r0, [r4]
	movs r1, #1
	bl sub_8006D50
	ldr r0, [r4]
	movs r1, #2
	bl sub_8006D50
	ldr r0, [r4]
	movs r1, #3
	bl sub_8006D50
	ldr r0, [r4]
	movs r7, #0
	adds r2, r0, #0
	adds r2, #0x6c
	ldr r6, _08034980 @ =gStaticData_0817C532
	adds r1, r0, #0
	adds r1, #0x2c
	ldr r5, _08034984 @ =gStaticData_0817C512
	ldr r4, _08034988 @ =gStaticData_0817C572
	ldr r3, _0803498C @ =gStaticData_0817C552
_0803491A:
	ldrh r0, [r5]
	strh r0, [r1]
	ldrh r0, [r6]
	strh r0, [r1, #0x20]
	ldrh r0, [r3]
	strh r0, [r2]
	ldrh r0, [r4]
	strh r0, [r2, #0x20]
	adds r2, #2
	adds r6, #2
	adds r1, #2
	adds r5, #2
	adds r4, #2
	adds r3, #2
	adds r7, #1
	cmp r7, #0xf
	ble _0803491A
	ldr r0, _0803497C @ =gUnknown_030012B8
	ldr r0, [r0]
	bl sub_8006DC8
	movs r0, #0x10
	mov r1, r8
	ldrb r1, [r1, #0xd]
	orrs r0, r1
	mov r2, r8
	strb r0, [r2, #0xd]
	ldr r4, _08034990 @ =gUnknown_03001300
	ldr r0, [r4]
	bl sub_8006A90
	ldr r0, [r4]
	bl sub_8006A48
	bl sub_80006A8
	ldr r0, [r4]
	bl sub_8006AAC
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08034974: .4byte gUnknown_030012FC
_08034978: .4byte gUnknown_030012DC
_0803497C: .4byte gUnknown_030012B8
_08034980: .4byte gStaticData_0817C532
_08034984: .4byte gStaticData_0817C512
_08034988: .4byte gStaticData_0817C572
_0803498C: .4byte gStaticData_0817C552
_08034990: .4byte gUnknown_03001300

.endif
