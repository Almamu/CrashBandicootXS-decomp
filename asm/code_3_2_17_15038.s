.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8015038 is reconstructed (but not yet byte-matching) as C in
@ src/graphics/actor_part28.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-18-0x08014f8c-actor.md, "Parked, not matched:
@ sub_8015038".
.if NON_MATCHING == 0
	thumb_func_start sub_8015038
sub_8015038: @ 0x08015038
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	adds r6, r0, #0
	mov ip, r1
	adds r3, r2, #0
	adds r0, #0x24
	ldrb r0, [r0]
	cmp r0, #0
	bne _080150E6
	movs r1, #0x17
	mov r8, r1
	adds r1, r6, #0
	adds r1, #0x21
	strb r0, [r1]
	adds r0, r6, #0
	adds r0, #0x22
	ldrb r2, [r0]
	adds r7, r1, #0
	adds r5, r0, #0
	cmp r2, #1
	bne _0801506C
	movs r0, #0x28
	mov r8, r0
	b _08015074
_0801506C:
	cmp r2, #2
	bne _08015076
	movs r1, #0x27
	mov r8, r1
_08015074:
	strb r2, [r7]
_08015076:
	movs r2, #0
	mov sb, r2
	movs r4, #0x14
	ldr r1, [r6, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x24]
	mov r1, ip
	bl sub_803AD80
	ldr r2, [r6, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r6, r0
	ldr r1, [r6, #0x10]
	ldr r3, [r2, #4]
	mov r2, r8
	bl sub_803AD84
	mov r2, sb
	str r2, [r6, #0x18]
	str r4, [r6, #0x1c]
	ldr r0, _080150DC @ =gUnknown_030012BC
	ldr r0, [r0]
	ldrb r1, [r7]
	adds r1, #0x57
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	ldrb r0, [r5]
	adds r0, #1
	strb r0, [r5]
	adds r1, r6, #0
	adds r1, #0x20
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	ldrb r1, [r1]
	cmp r0, r1
	blo _080151AE
	adds r0, r6, #0
	adds r0, #0x24
	movs r1, #1
	strb r1, [r0]
	ldrb r0, [r5]
	cmp r0, #1
	bls _080150E0
	strb r1, [r5]
	b _080151AE
	.align 2, 0
_080150DC: .4byte gUnknown_030012BC
_080150E0:
	mov r1, sb
	strb r1, [r5]
	b _080151AE
_080150E6:
	adds r0, r6, #0
	adds r0, #0x22
	adds r5, r0, #0
	ldrb r2, [r5]
	cmp r2, #0xf0
	bls _0801515C
	movs r0, #0x17
	mov r8, r0
	adds r1, r6, #0
	adds r1, #0x21
	movs r0, #0
	strb r0, [r1]
	ldrb r0, [r5]
	adds r7, r1, #0
	cmp r0, #1
	bne _0801510C
	movs r1, #0x28
	mov r8, r1
	b _08015114
_0801510C:
	cmp r0, #2
	bne _08015116
	movs r2, #0x27
	mov r8, r2
_08015114:
	strb r0, [r7]
_08015116:
	movs r4, #0
	movs r0, #0x14
	mov sb, r0
	ldr r1, [r6, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x24]
	mov r1, ip
	bl sub_803AD80
	ldr r2, [r6, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r6, r0
	ldr r1, [r6, #0x10]
	ldr r3, [r2, #4]
	mov r2, r8
	bl sub_803AD84
	str r4, [r6, #0x18]
	mov r2, sb
	str r2, [r6, #0x1c]
	ldr r0, _08015158 @ =gUnknown_030012BC
	ldr r0, [r0]
	ldrb r1, [r7]
	adds r1, #0x57
	movs r2, #0x80
	lsls r2, r2, #1
	bl PlaySfx
	b _080151A8
	.align 2, 0
_08015158: .4byte gUnknown_030012BC
_0801515C:
	adds r0, r6, #0
	adds r0, #0x21
	movs r4, #0
	strb r4, [r0]
	subs r0, #1
	strb r4, [r0]
	movs r7, #0x18
	ldr r1, [r6, #0xc]
	movs r2, #0x20
	ldrsh r0, [r1, r2]
	adds r0, r6, r0
	ldr r2, [r1, #0x24]
	adds r1, r3, #0
	bl sub_803AD80
	ldr r2, [r6, #0xc]
	adds r2, #0x50
	movs r1, #0
	ldrsh r0, [r2, r1]
	adds r0, r6, r0
	ldr r1, [r6, #0x10]
	ldr r3, [r2, #4]
	movs r2, #0x10
	bl sub_803AD84
	str r4, [r6, #0x18]
	str r7, [r6, #0x1c]
	ldr r0, _080151C4 @ =gUnknown_030012BC
	ldr r0, [r0]
	movs r2, #0x80
	lsls r2, r2, #1
	movs r1, #0xa
	bl PlaySfx
	adds r1, r6, #0
	adds r1, #0x26
	movs r0, #0x63
	strb r0, [r1]
_080151A8:
	ldrb r0, [r5]
	subs r0, #1
	strb r0, [r5]
_080151AE:
	adds r1, r6, #0
	adds r1, #0x23
	movs r0, #0
	strb r0, [r1]
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080151C4: .4byte gUnknown_030012BC

.endif
