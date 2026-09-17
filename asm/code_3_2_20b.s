.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_8037648
sub_8037648: @ 0x08037648
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x34
	movs r4, #0
	str r4, [sp]
	adds r5, r1, #0
	adds r4, r0, #0
	cmp r5, #0
	bge _08037678
	ldr r0, [sp]
	mvns r0, r0
	str r0, [sp]
	rsbs r0, r4, #0
	adds r6, r0, #0
	rsbs r1, r5, #0
	cmp r0, #0
	beq _08037672
	subs r1, #1
_08037672:
	adds r7, r1, #0
	adds r5, r7, #0
	adds r4, r6, #0
_08037678:
	cmp r3, #0
	bge _08037694
	ldr r1, [sp]
	mvns r1, r1
	str r1, [sp]
	rsbs r0, r2, #0
	str r0, [sp, #4]
	rsbs r2, r3, #0
	cmp r0, #0
	beq _0803768E
	subs r2, #1
_0803768E:
	str r2, [sp, #8]
	ldr r2, [sp, #4]
	ldr r3, [sp, #8]
_08037694:
	adds r7, r2, #0
	adds r6, r3, #0
	mov sl, r4
	mov r8, r5
	cmp r6, #0
	beq _080376A2
	b _080378F0
_080376A2:
	cmp r7, r8
	bls _0803777C
	ldr r0, _080376B8 @ =0x0000FFFF
	cmp r7, r0
	bhi _080376BC
	movs r1, #0
	cmp r7, #0xff
	bls _080376C6
	movs r1, #8
	b _080376C6
	.align 2, 0
_080376B8: .4byte 0x0000FFFF
_080376BC:
	ldr r0, _08037770 @ =0x00FFFFFF
	movs r1, #0x18
	cmp r7, r0
	bhi _080376C6
	movs r1, #0x10
_080376C6:
	ldr r0, _08037774 @ =gStaticData_085A4C70
	lsrs r2, r1
	adds r0, r2, r0
	ldrb r0, [r0]
	adds r0, r0, r1
	movs r1, #0x20
	subs r2, r1, r0
	cmp r2, #0
	beq _080376EE
	lsls r7, r2
	mov r3, r8
	lsls r3, r2
	subs r1, r1, r2
	mov r0, sl
	lsrs r0, r1
	orrs r3, r0
	mov r8, r3
	mov r4, sl
	lsls r4, r2
	mov sl, r4
_080376EE:
	lsrs r0, r7, #0x10
	mov sb, r0
	ldr r1, _08037778 @ =0x0000FFFF
	ands r1, r7
	str r1, [sp, #0xc]
	mov r0, r8
	mov r1, sb
	bl sub_803AF1C
	adds r4, r0, #0
	mov r0, r8
	mov r1, sb
	bl sub_8037E54
	adds r6, r0, #0
	ldr r3, [sp, #0xc]
	adds r2, r6, #0
	muls r2, r3, r2
	lsls r4, r4, #0x10
	mov r1, sl
	lsrs r0, r1, #0x10
	orrs r4, r0
	cmp r4, r2
	bhs _0803772E
	subs r6, #1
	adds r4, r4, r7
	cmp r4, r7
	blo _0803772E
	cmp r4, r2
	bhs _0803772E
	subs r6, #1
	adds r4, r4, r7
_0803772E:
	subs r4, r4, r2
	adds r0, r4, #0
	mov r1, sb
	bl sub_803AF1C
	adds r5, r0, #0
	adds r0, r4, #0
	mov r1, sb
	bl sub_8037E54
	adds r1, r0, #0
	ldr r3, [sp, #0xc]
	adds r2, r1, #0
	muls r2, r3, r2
	lsls r5, r5, #0x10
	ldr r0, _08037778 @ =0x0000FFFF
	mov r4, sl
	ands r4, r0
	orrs r5, r4
	cmp r5, r2
	bhs _08037766
	subs r1, #1
	adds r5, r5, r7
	cmp r5, r7
	blo _08037766
	cmp r5, r2
	bhs _08037766
	subs r1, #1
_08037766:
	lsls r6, r6, #0x10
	orrs r6, r1
	movs r0, #0
	str r0, [sp, #0x10]
	b _08037A46
	.align 2, 0
_08037770: .4byte 0x00FFFFFF
_08037774: .4byte gStaticData_085A4C70
_08037778: .4byte 0x0000FFFF
_0803777C:
	cmp r2, #0
	bne _0803778A
	movs r0, #1
	movs r1, #0
	bl sub_8037E54
	adds r7, r0, #0
_0803778A:
	adds r1, r7, #0
	ldr r0, _0803779C @ =0x0000FFFF
	cmp r7, r0
	bhi _080377A0
	movs r2, #0
	cmp r7, #0xff
	bls _080377AA
	movs r2, #8
	b _080377AA
	.align 2, 0
_0803779C: .4byte 0x0000FFFF
_080377A0:
	ldr r0, _080377C8 @ =0x00FFFFFF
	movs r2, #0x18
	cmp r7, r0
	bhi _080377AA
	movs r2, #0x10
_080377AA:
	ldr r0, _080377CC @ =gStaticData_085A4C70
	lsrs r1, r2
	adds r0, r1, r0
	ldrb r0, [r0]
	adds r0, r0, r2
	movs r1, #0x20
	subs r2, r1, r0
	cmp r2, #0
	bne _080377D0
	mov r1, r8
	subs r1, r1, r7
	mov r8, r1
	movs r2, #1
	str r2, [sp, #0x10]
	b _0803786E
	.align 2, 0
_080377C8: .4byte 0x00FFFFFF
_080377CC: .4byte gStaticData_085A4C70
_080377D0:
	subs r1, r1, r2
	lsls r7, r2
	mov r5, r8
	lsrs r5, r1
	mov r3, r8
	lsls r3, r2
	mov r0, sl
	lsrs r0, r1
	orrs r3, r0
	mov r8, r3
	mov r4, sl
	lsls r4, r2
	mov sl, r4
	lsrs r0, r7, #0x10
	mov sb, r0
	ldr r1, _080378EC @ =0x0000FFFF
	ands r1, r7
	str r1, [sp, #0x14]
	adds r0, r5, #0
	mov r1, sb
	bl sub_803AF1C
	adds r4, r0, #0
	adds r0, r5, #0
	mov r1, sb
	bl sub_8037E54
	adds r6, r0, #0
	ldr r2, [sp, #0x14]
	adds r1, r6, #0
	muls r1, r2, r1
	lsls r4, r4, #0x10
	mov r3, r8
	lsrs r0, r3, #0x10
	orrs r4, r0
	cmp r4, r1
	bhs _0803782A
	subs r6, #1
	adds r4, r4, r7
	cmp r4, r7
	blo _0803782A
	cmp r4, r1
	bhs _0803782A
	subs r6, #1
	adds r4, r4, r7
_0803782A:
	subs r4, r4, r1
	adds r0, r4, #0
	mov r1, sb
	bl sub_803AF1C
	adds r5, r0, #0
	adds r0, r4, #0
	mov r1, sb
	bl sub_8037E54
	adds r2, r0, #0
	ldr r4, [sp, #0x14]
	adds r1, r2, #0
	muls r1, r4, r1
	lsls r5, r5, #0x10
	ldr r0, _080378EC @ =0x0000FFFF
	mov r3, r8
	ands r3, r0
	orrs r5, r3
	cmp r5, r1
	bhs _08037864
	subs r2, #1
	adds r5, r5, r7
	cmp r5, r7
	blo _08037864
	cmp r5, r1
	bhs _08037864
	subs r2, #1
	adds r5, r5, r7
_08037864:
	lsls r6, r6, #0x10
	orrs r6, r2
	str r6, [sp, #0x10]
	subs r1, r5, r1
	mov r8, r1
_0803786E:
	lsrs r4, r7, #0x10
	mov sb, r4
	ldr r0, _080378EC @ =0x0000FFFF
	ands r0, r7
	str r0, [sp, #0x18]
	mov r0, r8
	mov r1, sb
	bl sub_803AF1C
	adds r4, r0, #0
	mov r0, r8
	mov r1, sb
	bl sub_8037E54
	adds r6, r0, #0
	ldr r1, [sp, #0x18]
	adds r2, r6, #0
	muls r2, r1, r2
	lsls r4, r4, #0x10
	mov r3, sl
	lsrs r0, r3, #0x10
	orrs r4, r0
	cmp r4, r2
	bhs _080378AE
	subs r6, #1
	adds r4, r4, r7
	cmp r4, r7
	blo _080378AE
	cmp r4, r2
	bhs _080378AE
	subs r6, #1
	adds r4, r4, r7
_080378AE:
	subs r4, r4, r2
	adds r0, r4, #0
	mov r1, sb
	bl sub_803AF1C
	adds r5, r0, #0
	adds r0, r4, #0
	mov r1, sb
	bl sub_8037E54
	adds r1, r0, #0
	ldr r4, [sp, #0x18]
	adds r2, r1, #0
	muls r2, r4, r2
	lsls r5, r5, #0x10
	ldr r0, _080378EC @ =0x0000FFFF
	mov r3, sl
	ands r3, r0
	orrs r5, r3
	cmp r5, r2
	bhs _080378E6
	subs r1, #1
	adds r5, r5, r7
	cmp r5, r7
	blo _080378E6
	cmp r5, r2
	bhs _080378E6
	subs r1, #1
_080378E6:
	lsls r6, r6, #0x10
	orrs r6, r1
	b _08037A46
	.align 2, 0
_080378EC: .4byte 0x0000FFFF
_080378F0:
	cmp r6, r8
	bls _080378FC
	movs r6, #0
	movs r4, #0
	str r4, [sp, #0x10]
	b _08037A46
_080378FC:
	adds r1, r6, #0
	ldr r0, _08037910 @ =0x0000FFFF
	cmp r6, r0
	bhi _08037914
	movs r2, #0
	cmp r6, #0xff
	bls _0803791E
	movs r2, #8
	b _0803791E
	.align 2, 0
_08037910: .4byte 0x0000FFFF
_08037914:
	ldr r0, _08037940 @ =0x00FFFFFF
	movs r2, #0x18
	cmp r6, r0
	bhi _0803791E
	movs r2, #0x10
_0803791E:
	ldr r0, _08037944 @ =gStaticData_085A4C70
	lsrs r1, r2
	adds r0, r1, r0
	ldrb r0, [r0]
	adds r0, r0, r2
	movs r1, #0x20
	subs r2, r1, r0
	cmp r2, #0
	bne _0803794C
	cmp r8, r6
	bhi _08037938
	cmp sl, r7
	blo _08037948
_08037938:
	movs r6, #1
	mov r1, sl
	b _08037A40
	.align 2, 0
_08037940: .4byte 0x00FFFFFF
_08037944: .4byte gStaticData_085A4C70
_08037948:
	movs r6, #0
	b _08037A42
_0803794C:
	subs r1, r1, r2
	lsls r6, r2
	adds r0, r7, #0
	lsrs r0, r1
	orrs r6, r0
	lsls r7, r2
	mov r5, r8
	lsrs r5, r1
	mov r3, r8
	lsls r3, r2
	mov r0, sl
	lsrs r0, r1
	orrs r3, r0
	mov r8, r3
	mov r4, sl
	lsls r4, r2
	mov sl, r4
	lsrs r0, r6, #0x10
	mov sb, r0
	ldr r1, _08037A78 @ =0x0000FFFF
	ands r1, r6
	str r1, [sp, #0x1c]
	adds r0, r5, #0
	mov r1, sb
	bl sub_803AF1C
	adds r4, r0, #0
	adds r0, r5, #0
	mov r1, sb
	bl sub_8037E54
	adds r3, r0, #0
	ldr r2, [sp, #0x1c]
	adds r1, r3, #0
	muls r1, r2, r1
	lsls r4, r4, #0x10
	mov r2, r8
	lsrs r0, r2, #0x10
	orrs r4, r0
	cmp r4, r1
	bhs _080379AE
	subs r3, #1
	adds r4, r4, r6
	cmp r4, r6
	blo _080379AE
	cmp r4, r1
	bhs _080379AE
	subs r3, #1
	adds r4, r4, r6
_080379AE:
	subs r4, r4, r1
	adds r0, r4, #0
	mov r1, sb
	str r3, [sp, #0x30]
	bl sub_803AF1C
	adds r5, r0, #0
	adds r0, r4, #0
	mov r1, sb
	bl sub_8037E54
	adds r2, r0, #0
	ldr r4, [sp, #0x1c]
	adds r1, r2, #0
	muls r1, r4, r1
	lsls r5, r5, #0x10
	ldr r0, _08037A78 @ =0x0000FFFF
	mov r4, r8
	ands r4, r0
	orrs r5, r4
	ldr r3, [sp, #0x30]
	cmp r5, r1
	bhs _080379EC
	subs r2, #1
	adds r5, r5, r6
	cmp r5, r6
	blo _080379EC
	cmp r5, r1
	bhs _080379EC
	subs r2, #1
	adds r5, r5, r6
_080379EC:
	lsls r6, r3, #0x10
	orrs r6, r2
	subs r1, r5, r1
	mov r8, r1
	ldr r0, _08037A78 @ =0x0000FFFF
	mov sb, r0
	adds r1, r6, #0
	ands r1, r0
	lsrs r3, r6, #0x10
	adds r0, r7, #0
	mov r2, sb
	ands r0, r2
	lsrs r2, r7, #0x10
	adds r5, r1, #0
	muls r5, r0, r5
	adds r4, r1, #0
	muls r4, r2, r4
	adds r1, r3, #0
	muls r1, r0, r1
	muls r3, r2, r3
	lsrs r0, r5, #0x10
	adds r4, r4, r0
	adds r4, r4, r1
	cmp r4, r1
	bhs _08037A24
	movs r0, #0x80
	lsls r0, r0, #9
	adds r3, r3, r0
_08037A24:
	lsrs r0, r4, #0x10
	adds r3, r3, r0
	mov r1, sb
	ands r4, r1
	lsls r0, r4, #0x10
	ands r5, r1
	adds r1, r0, r5
	cmp r3, r8
	bhi _08037A3E
	cmp r3, r8
	bne _08037A42
	cmp r1, sl
	bls _08037A42
_08037A3E:
	subs r6, #1
_08037A40:
	subs r0, r1, r7
_08037A42:
	movs r2, #0
	str r2, [sp, #0x10]
_08037A46:
	str r6, [sp, #0x20]
	ldr r3, [sp, #0x10]
	str r3, [sp, #0x24]
	ldr r1, [sp, #0x20]
	ldr r2, [sp, #0x24]
	ldr r4, [sp]
	cmp r4, #0
	beq _08037A68
	rsbs r0, r1, #0
	str r0, [sp, #0x28]
	rsbs r1, r2, #0
	cmp r0, #0
	beq _08037A62
	subs r1, #1
_08037A62:
	str r1, [sp, #0x2c]
	ldr r1, [sp, #0x28]
	ldr r2, [sp, #0x2c]
_08037A68:
	adds r0, r1, #0
	adds r1, r2, #0
	add sp, #0x34
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7, pc}
	.align 2, 0
_08037A78: .4byte 0x0000FFFF

	thumb_func_start sub_8037A7C
sub_8037A7C: @ 0x08037A7C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x20
	adds r7, r2, #0
	adds r6, r3, #0
	mov sl, r0
	mov r8, r1
	cmp r6, #0
	beq _08037A96
	b _08037CE4
_08037A96:
	cmp r7, r8
	bls _08037B70
	ldr r0, _08037AAC @ =0x0000FFFF
	cmp r7, r0
	bhi _08037AB0
	movs r1, #0
	cmp r7, #0xff
	bls _08037ABA
	movs r1, #8
	b _08037ABA
	.align 2, 0
_08037AAC: .4byte 0x0000FFFF
_08037AB0:
	ldr r0, _08037B64 @ =0x00FFFFFF
	movs r1, #0x18
	cmp r7, r0
	bhi _08037ABA
	movs r1, #0x10
_08037ABA:
	ldr r0, _08037B68 @ =gStaticData_085A4D70
	lsrs r2, r1
	adds r0, r2, r0
	ldrb r0, [r0]
	adds r0, r0, r1
	movs r1, #0x20
	subs r2, r1, r0
	cmp r2, #0
	beq _08037AE6
	lsls r7, r2
	mov r0, r8
	lsls r0, r2
	mov r8, r0
	subs r1, r1, r2
	mov r0, sl
	lsrs r0, r1
	mov r1, r8
	orrs r1, r0
	mov r8, r1
	mov r3, sl
	lsls r3, r2
	mov sl, r3
_08037AE6:
	lsrs r4, r7, #0x10
	mov sb, r4
	ldr r0, _08037B6C @ =0x0000FFFF
	ands r0, r7
	str r0, [sp]
	mov r0, r8
	mov r1, sb
	bl sub_803AF1C
	adds r4, r0, #0
	mov r0, r8
	mov r1, sb
	bl sub_8037E54
	adds r6, r0, #0
	ldr r1, [sp]
	adds r2, r6, #0
	muls r2, r1, r2
	lsls r4, r4, #0x10
	mov r3, sl
	lsrs r0, r3, #0x10
	orrs r4, r0
	cmp r4, r2
	bhs _08037B26
	subs r6, #1
	adds r4, r4, r7
	cmp r4, r7
	blo _08037B26
	cmp r4, r2
	bhs _08037B26
	subs r6, #1
	adds r4, r4, r7
_08037B26:
	subs r4, r4, r2
	adds r0, r4, #0
	mov r1, sb
	bl sub_803AF1C
	adds r5, r0, #0
	adds r0, r4, #0
	mov r1, sb
	bl sub_8037E54
	adds r1, r0, #0
	ldr r4, [sp]
	adds r2, r1, #0
	muls r2, r4, r2
	lsls r5, r5, #0x10
	ldr r0, _08037B6C @ =0x0000FFFF
	mov r3, sl
	ands r3, r0
	orrs r5, r3
	cmp r5, r2
	bhs _08037B5E
	subs r1, #1
	adds r5, r5, r7
	cmp r5, r7
	blo _08037B5E
	cmp r5, r2
	bhs _08037B5E
	subs r1, #1
_08037B5E:
	lsls r6, r6, #0x10
	orrs r6, r1
	b _08037CEA
	.align 2, 0
_08037B64: .4byte 0x00FFFFFF
_08037B68: .4byte gStaticData_085A4D70
_08037B6C: .4byte 0x0000FFFF
_08037B70:
	cmp r2, #0
	bne _08037B7E
	movs r0, #1
	movs r1, #0
	bl sub_8037E54
	adds r7, r0, #0
_08037B7E:
	adds r1, r7, #0
	ldr r0, _08037B90 @ =0x0000FFFF
	cmp r7, r0
	bhi _08037B94
	movs r2, #0
	cmp r7, #0xff
	bls _08037B9E
	movs r2, #8
	b _08037B9E
	.align 2, 0
_08037B90: .4byte 0x0000FFFF
_08037B94:
	ldr r0, _08037BBC @ =0x00FFFFFF
	movs r2, #0x18
	cmp r7, r0
	bhi _08037B9E
	movs r2, #0x10
_08037B9E:
	ldr r0, _08037BC0 @ =gStaticData_085A4D70
	lsrs r1, r2
	adds r0, r1, r0
	ldrb r0, [r0]
	adds r0, r0, r2
	movs r1, #0x20
	subs r2, r1, r0
	cmp r2, #0
	bne _08037BC4
	mov r0, r8
	subs r0, r0, r7
	mov r8, r0
	movs r1, #1
	str r1, [sp, #4]
	b _08037C62
	.align 2, 0
_08037BBC: .4byte 0x00FFFFFF
_08037BC0: .4byte gStaticData_085A4D70
_08037BC4:
	subs r1, r1, r2
	lsls r7, r2
	mov r5, r8
	lsrs r5, r1
	mov r3, r8
	lsls r3, r2
	mov r0, sl
	lsrs r0, r1
	orrs r3, r0
	mov r8, r3
	mov r4, sl
	lsls r4, r2
	mov sl, r4
	lsrs r0, r7, #0x10
	mov sb, r0
	ldr r1, _08037CE0 @ =0x0000FFFF
	ands r1, r7
	str r1, [sp, #8]
	adds r0, r5, #0
	mov r1, sb
	bl sub_803AF1C
	adds r4, r0, #0
	adds r0, r5, #0
	mov r1, sb
	bl sub_8037E54
	adds r6, r0, #0
	ldr r2, [sp, #8]
	adds r1, r6, #0
	muls r1, r2, r1
	lsls r4, r4, #0x10
	mov r3, r8
	lsrs r0, r3, #0x10
	orrs r4, r0
	cmp r4, r1
	bhs _08037C1E
	subs r6, #1
	adds r4, r4, r7
	cmp r4, r7
	blo _08037C1E
	cmp r4, r1
	bhs _08037C1E
	subs r6, #1
	adds r4, r4, r7
_08037C1E:
	subs r4, r4, r1
	adds r0, r4, #0
	mov r1, sb
	bl sub_803AF1C
	adds r5, r0, #0
	adds r0, r4, #0
	mov r1, sb
	bl sub_8037E54
	adds r2, r0, #0
	ldr r4, [sp, #8]
	adds r1, r2, #0
	muls r1, r4, r1
	lsls r5, r5, #0x10
	ldr r0, _08037CE0 @ =0x0000FFFF
	mov r3, r8
	ands r3, r0
	orrs r5, r3
	cmp r5, r1
	bhs _08037C58
	subs r2, #1
	adds r5, r5, r7
	cmp r5, r7
	blo _08037C58
	cmp r5, r1
	bhs _08037C58
	subs r2, #1
	adds r5, r5, r7
_08037C58:
	lsls r6, r6, #0x10
	orrs r6, r2
	str r6, [sp, #4]
	subs r1, r5, r1
	mov r8, r1
_08037C62:
	lsrs r4, r7, #0x10
	mov sb, r4
	ldr r0, _08037CE0 @ =0x0000FFFF
	ands r0, r7
	str r0, [sp, #0xc]
	mov r0, r8
	mov r1, sb
	bl sub_803AF1C
	adds r4, r0, #0
	mov r0, r8
	mov r1, sb
	bl sub_8037E54
	adds r6, r0, #0
	ldr r1, [sp, #0xc]
	adds r2, r6, #0
	muls r2, r1, r2
	lsls r4, r4, #0x10
	mov r3, sl
	lsrs r0, r3, #0x10
	orrs r4, r0
	cmp r4, r2
	bhs _08037CA2
	subs r6, #1
	adds r4, r4, r7
	cmp r4, r7
	blo _08037CA2
	cmp r4, r2
	bhs _08037CA2
	subs r6, #1
	adds r4, r4, r7
_08037CA2:
	subs r4, r4, r2
	adds r0, r4, #0
	mov r1, sb
	bl sub_803AF1C
	adds r5, r0, #0
	adds r0, r4, #0
	mov r1, sb
	bl sub_8037E54
	adds r1, r0, #0
	ldr r4, [sp, #0xc]
	adds r2, r1, #0
	muls r2, r4, r2
	lsls r5, r5, #0x10
	ldr r0, _08037CE0 @ =0x0000FFFF
	mov r3, sl
	ands r3, r0
	orrs r5, r3
	cmp r5, r2
	bhs _08037CDA
	subs r1, #1
	adds r5, r5, r7
	cmp r5, r7
	blo _08037CDA
	cmp r5, r2
	bhs _08037CDA
	subs r1, #1
_08037CDA:
	lsls r6, r6, #0x10
	orrs r6, r1
	b _08037E3A
	.align 2, 0
_08037CE0: .4byte 0x0000FFFF
_08037CE4:
	cmp r6, r8
	bls _08037CF0
	movs r6, #0
_08037CEA:
	movs r4, #0
	str r4, [sp, #4]
	b _08037E3A
_08037CF0:
	adds r1, r6, #0
	ldr r0, _08037D04 @ =0x0000FFFF
	cmp r6, r0
	bhi _08037D08
	movs r2, #0
	cmp r6, #0xff
	bls _08037D12
	movs r2, #8
	b _08037D12
	.align 2, 0
_08037D04: .4byte 0x0000FFFF
_08037D08:
	ldr r0, _08037D34 @ =0x00FFFFFF
	movs r2, #0x18
	cmp r6, r0
	bhi _08037D12
	movs r2, #0x10
_08037D12:
	ldr r0, _08037D38 @ =gStaticData_085A4D70
	lsrs r1, r2
	adds r0, r1, r0
	ldrb r0, [r0]
	adds r0, r0, r2
	movs r1, #0x20
	subs r2, r1, r0
	cmp r2, #0
	bne _08037D40
	cmp r8, r6
	bhi _08037D2C
	cmp sl, r7
	blo _08037D3C
_08037D2C:
	movs r6, #1
	mov r1, sl
	b _08037E34
	.align 2, 0
_08037D34: .4byte 0x00FFFFFF
_08037D38: .4byte gStaticData_085A4D70
_08037D3C:
	movs r6, #0
	b _08037E36
_08037D40:
	subs r1, r1, r2
	lsls r6, r2
	adds r0, r7, #0
	lsrs r0, r1
	orrs r6, r0
	lsls r7, r2
	mov r5, r8
	lsrs r5, r1
	mov r3, r8
	lsls r3, r2
	mov r0, sl
	lsrs r0, r1
	orrs r3, r0
	mov r8, r3
	mov r4, sl
	lsls r4, r2
	mov sl, r4
	lsrs r0, r6, #0x10
	mov sb, r0
	ldr r1, _08037E50 @ =0x0000FFFF
	ands r1, r6
	str r1, [sp, #0x10]
	adds r0, r5, #0
	mov r1, sb
	bl sub_803AF1C
	adds r4, r0, #0
	adds r0, r5, #0
	mov r1, sb
	bl sub_8037E54
	adds r3, r0, #0
	ldr r2, [sp, #0x10]
	adds r1, r3, #0
	muls r1, r2, r1
	lsls r4, r4, #0x10
	mov r2, r8
	lsrs r0, r2, #0x10
	orrs r4, r0
	cmp r4, r1
	bhs _08037DA2
	subs r3, #1
	adds r4, r4, r6
	cmp r4, r6
	blo _08037DA2
	cmp r4, r1
	bhs _08037DA2
	subs r3, #1
	adds r4, r4, r6
_08037DA2:
	subs r4, r4, r1
	adds r0, r4, #0
	mov r1, sb
	str r3, [sp, #0x1c]
	bl sub_803AF1C
	adds r5, r0, #0
	adds r0, r4, #0
	mov r1, sb
	bl sub_8037E54
	adds r2, r0, #0
	ldr r4, [sp, #0x10]
	adds r1, r2, #0
	muls r1, r4, r1
	lsls r5, r5, #0x10
	ldr r0, _08037E50 @ =0x0000FFFF
	mov r4, r8
	ands r4, r0
	orrs r5, r4
	ldr r3, [sp, #0x1c]
	cmp r5, r1
	bhs _08037DE0
	subs r2, #1
	adds r5, r5, r6
	cmp r5, r6
	blo _08037DE0
	cmp r5, r1
	bhs _08037DE0
	subs r2, #1
	adds r5, r5, r6
_08037DE0:
	lsls r6, r3, #0x10
	orrs r6, r2
	subs r1, r5, r1
	mov r8, r1
	ldr r0, _08037E50 @ =0x0000FFFF
	mov sb, r0
	adds r1, r6, #0
	ands r1, r0
	lsrs r3, r6, #0x10
	adds r0, r7, #0
	mov r2, sb
	ands r0, r2
	lsrs r2, r7, #0x10
	adds r5, r1, #0
	muls r5, r0, r5
	adds r4, r1, #0
	muls r4, r2, r4
	adds r1, r3, #0
	muls r1, r0, r1
	muls r3, r2, r3
	lsrs r0, r5, #0x10
	adds r4, r4, r0
	adds r4, r4, r1
	cmp r4, r1
	bhs _08037E18
	movs r0, #0x80
	lsls r0, r0, #9
	adds r3, r3, r0
_08037E18:
	lsrs r0, r4, #0x10
	adds r3, r3, r0
	mov r1, sb
	ands r4, r1
	lsls r0, r4, #0x10
	ands r5, r1
	adds r1, r0, r5
	cmp r3, r8
	bhi _08037E32
	cmp r3, r8
	bne _08037E36
	cmp r1, sl
	bls _08037E36
_08037E32:
	subs r6, #1
_08037E34:
	subs r0, r1, r7
_08037E36:
	movs r2, #0
	str r2, [sp, #4]
_08037E3A:
	str r6, [sp, #0x14]
	ldr r3, [sp, #4]
	str r3, [sp, #0x18]
	ldr r0, [sp, #0x14]
	ldr r1, [sp, #0x18]
	add sp, #0x20
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7, pc}
	.align 2, 0
_08037E50: .4byte 0x0000FFFF

	thumb_func_start sub_8037E54
sub_8037E54: @ 0x08037E54
	cmp r1, #0
	beq _08037EC2
	movs r3, #1
	movs r2, #0
	push {r4}
	cmp r0, r1
	blo _08037EBC
	movs r4, #1
	lsls r4, r4, #0x1c
_08037E66:
	cmp r1, r4
	bhs _08037E74
	cmp r1, r0
	bhs _08037E74
	lsls r1, r1, #4
	lsls r3, r3, #4
	b _08037E66
_08037E74:
	lsls r4, r4, #3
_08037E76:
	cmp r1, r4
	bhs _08037E84
	cmp r1, r0
	bhs _08037E84
	lsls r1, r1, #1
	lsls r3, r3, #1
	b _08037E76
_08037E84:
	cmp r0, r1
	blo _08037E8C
	subs r0, r0, r1
	orrs r2, r3
_08037E8C:
	lsrs r4, r1, #1
	cmp r0, r4
	blo _08037E98
	subs r0, r0, r4
	lsrs r4, r3, #1
	orrs r2, r4
_08037E98:
	lsrs r4, r1, #2
	cmp r0, r4
	blo _08037EA4
	subs r0, r0, r4
	lsrs r4, r3, #2
	orrs r2, r4
_08037EA4:
	lsrs r4, r1, #3
	cmp r0, r4
	blo _08037EB0
	subs r0, r0, r4
	lsrs r4, r3, #3
	orrs r2, r4
_08037EB0:
	cmp r0, #0
	beq _08037EBC
	lsrs r3, r3, #4
	beq _08037EBC
	lsrs r1, r1, #4
	b _08037E84
_08037EBC:
	adds r0, r2, #0
	pop {r4}
	mov pc, lr
_08037EC2:
	push {lr}
	bl nullsub_8
	movs r0, #0
	pop {pc}

	thumb_func_start sub_8037ECC
sub_8037ECC: @ 0x08037ECC
	push {r4, r5, r6, r7, lr}
	sub sp, #0x10
	str r0, [sp]
	str r1, [sp, #4]
	str r2, [sp, #8]
	str r3, [sp, #0xc]
	ldr r3, [sp]
	ldr r0, _08037F38 @ =0x0000FFFF
	mov ip, r0
	adds r2, r3, #0
	ands r2, r0
	lsrs r3, r3, #0x10
	ldr r1, [sp, #8]
	adds r0, r1, #0
	mov r4, ip
	ands r0, r4
	lsrs r1, r1, #0x10
	adds r5, r2, #0
	muls r5, r0, r5
	adds r4, r2, #0
	muls r4, r1, r4
	adds r2, r3, #0
	muls r2, r0, r2
	muls r3, r1, r3
	lsrs r0, r5, #0x10
	adds r4, r4, r0
	adds r4, r4, r2
	cmp r4, r2
	bhs _08037F0C
	movs r0, #0x80
	lsls r0, r0, #9
	adds r3, r3, r0
_08037F0C:
	lsrs r0, r4, #0x10
	adds r7, r3, r0
	mov r1, ip
	ands r4, r1
	lsls r0, r4, #0x10
	ands r5, r1
	adds r6, r0, #0
	orrs r6, r5
	adds r1, r7, #0
	adds r0, r6, #0
	ldr r3, [sp]
	ldr r4, [sp, #0xc]
	adds r2, r3, #0
	muls r2, r4, r2
	ldr r5, [sp, #4]
	ldr r4, [sp, #8]
	adds r3, r5, #0
	muls r3, r4, r3
	adds r2, r2, r3
	adds r1, r7, r2
	add sp, #0x10
	pop {r4, r5, r6, r7, pc}
	.align 2, 0
_08037F38: .4byte 0x0000FFFF

	thumb_func_start sub_8037F3C
sub_8037F3C: @ 0x08037F3C
	push {r4, r5, r6, lr}
	sub sp, #4
	adds r4, r0, #0
	adds r6, r1, #0
	movs r0, #3
	ands r0, r4
	cmp r0, #0
	beq _08037F5E
	movs r2, #0
	movs r1, #3
_08037F50:
	strb r2, [r4]
	adds r4, #1
	subs r6, #1
	adds r0, r4, #0
	ands r0, r1
	cmp r0, #0
	bne _08037F50
_08037F5E:
	movs r0, #0
	str r0, [sp]
	movs r5, #0x20
	rsbs r5, r5, #0
	ands r5, r6
	adds r2, r5, #0
	cmp r5, #0
	bge _08037F70
	adds r2, r5, #3
_08037F70:
	lsls r2, r2, #9
	lsrs r2, r2, #0xb
	movs r0, #0x80
	lsls r0, r0, #0x11
	orrs r2, r0
	mov r0, sp
	adds r1, r4, #0
	bl sub_803A948
	adds r0, r4, r5
	subs r1, r6, r5
	cmp r1, #0
	ble _08037F96
	movs r2, #0
_08037F8C:
	strb r2, [r0]
	adds r0, #1
	subs r1, #1
	cmp r1, #0
	bne _08037F8C
_08037F96:
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0

