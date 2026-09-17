.include "asm/macros.inc"

.syntax unified
.arm

@ sub_8003698 is reconstructed (semantics fully understood) but NOT YET
@ byte-matching - parked as C in src/graphics/settings_menu8b.c, guarded
@ by #if NON_MATCHING. See docs/matching/issue-5-overlay-ui-sync.md.
.if NON_MATCHING == 0
	thumb_func_start sub_8003698
sub_8003698: @ 0x08003698
	push {r4, r5, r6, r7, lr}
	mov r7, sb
	mov r6, r8
	push {r6, r7}
	sub sp, #0xe0
	adds r7, r0, #0
	adds r6, r1, #0
	adds r4, r7, #0
	adds r4, #0x8c
	ldr r0, [r4]
	bl sub_8002CE8
	lsls r0, r0, #0x18
	cmp r0, #0
	bne _080036C6
	ldr r0, [r4]
	adds r1, r6, #0
	mov r2, sp
	bl sub_8002C14
	movs r0, #0
	mov sb, r0
	b _080036CA
_080036C6:
	movs r1, #1
	mov sb, r1
_080036CA:
	ldr r0, _0800372C @ =gUnknown_030012C0
	mov r8, r0
	ldr r0, [r0]
	bl sub_80236EC
	adds r1, r0, #0
	add r5, sp, #0x70
	adds r0, r5, #0
	movs r2, #0x68
	bl sub_800014C
	mov r1, r8
	ldr r0, [r1]
	bl sub_802332C
	add r1, sp, #0xd8
	strb r0, [r1]
	ldr r4, _08003730 @ =gUnknown_030012BC
	ldr r0, [r4]
	bl sub_8001ABC
	mov r1, sp
	adds r1, #0xda
	strh r0, [r1]
	ldr r0, [r4]
	bl sub_8001AC0
	add r1, sp, #0xdc
	strh r0, [r1]
	adds r4, r7, #0
	adds r4, #0x8c
	ldr r0, [r4]
	adds r1, r6, #0
	adds r2, r5, #0
	bl sub_8002C40
	ldr r0, [r4]
	bl sub_8002BA4
	cmp r0, #0
	beq _08003740
	mov r0, sb
	cmp r0, #0
	beq _08003734
	ldr r0, [r4]
	adds r1, r6, #0
	bl sub_8002C6C
	b _0800375C
	.align 2, 0
_0800372C: .4byte gUnknown_030012C0
_08003730: .4byte gUnknown_030012BC
_08003734:
	ldr r0, [r4]
	adds r1, r6, #0
	mov r2, sp
	bl sub_8002C40
	b _0800375C
_08003740:
	lsls r4, r6, #2
	adds r4, r4, r6
	lsls r4, r4, #2
	adds r4, #0x3c
	adds r4, r7, r4
	mov r1, r8
	ldr r0, [r1]
	bl sub_80236EC
	adds r2, r0, #0
	adds r0, r7, #0
	adds r1, r4, #0
	bl sub_80048E0
_0800375C:
	add sp, #0xe0
	pop {r3, r4}
	mov r8, r3
	mov sb, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
.endif
