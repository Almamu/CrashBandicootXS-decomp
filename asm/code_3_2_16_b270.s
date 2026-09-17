.include "asm/macros.inc"

.syntax unified
.arm

@ sub_800B270 is reconstructed (but not yet byte-matching) as C in
@ src/graphics/actor_part49.c, guarded by #if NON_MATCHING - this raw
@ version is only assembled for the default (matching) build. See
@ docs/matching/issue-9-0x08007634-actor.md.
.if NON_MATCHING == 0
	thumb_func_start sub_800B270
sub_800B270: @ 0x0800B270
	adds	r2, r0, #0
	ldr	r1, [r2, #96]	@ 0x60
	ldr	r3, [r2, #80]	@ 0x50
	cmp	r1, r3
	bge _0800B286
	ldr	r0, [r2, #76]	@ 0x4c
	adds	r0, r1, r0
	str	r0, [r2, #96]	@ 0x60
	cmp	r0, r3
	ble _0800B296
	b _0800B294
_0800B286:
	cmp	r1, r3
	ble _0800B296
	ldr	r0, [r2, #76]	@ 0x4c
	subs	r0, r1, r0
	str	r0, [r2, #96]	@ 0x60
	cmp	r0, r3
	bge _0800B296
_0800B294:
	str	r3, [r2, #96]	@ 0x60
_0800B296:
	ldr	r1, [r2, #100]	@ 0x64
	ldr	r3, [r2, #92]	@ 0x5c
	cmp	r1, r3
	bge _0800B2AA
	ldr	r0, [r2, #88]	@ 0x58
	adds	r0, r1, r0
	str	r0, [r2, #100]	@ 0x64
	cmp	r0, r3
	ble _0800B2BA
	b _0800B2B8
_0800B2AA:
	cmp	r1, r3
	ble _0800B2BA
	ldr	r0, [r2, #88]	@ 0x58
	subs	r0, r1, r0
	str	r0, [r2, #100]	@ 0x64
	cmp	r0, r3
	bge _0800B2BA
_0800B2B8:
	str	r3, [r2, #100]	@ 0x64
_0800B2BA:
	adds	r1, r2, #0
	adds	r1, #36	@ 0x24
	movs	r0, #0
	strb	r0, [r1, #0]
	ldr	r0, [r2, #96]	@ 0x60
	cmp	r0, #0
	ble _0800B2CC
	movs	r0, #1
	b _0800B2D2
_0800B2CC:
	cmp	r0, #0
	bge _0800B2D4
	movs	r0, #2
_0800B2D2:
	strb	r0, [r1, #0]
_0800B2D4:
	ldr	r0, [r2, #100]	@ 0x64
	cmp	r0, #0
	ble _0800B2DE
	movs	r0, #8
	b _0800B2E4
_0800B2DE:
	cmp	r0, #0
	bge _0800B2EA
	movs	r0, #4
_0800B2E4:
	ldrb	r3, [r1, #0]
	orrs	r0, r3
	strb	r0, [r1, #0]
_0800B2EA:
	ldr	r0, [r2, #0]
	ldr	r1, [r2, #4]
	str	r0, [r2, #108]	@ 0x6c
	str	r1, [r2, #112]	@ 0x70
	ldr	r0, [r2, #0]
	ldr	r3, [r2, #96]	@ 0x60
	adds	r0, r0, r3
	str	r0, [r2, #0]
	ldr	r0, [r2, #4]
	ldr	r1, [r2, #100]	@ 0x64
	adds	r0, r0, r1
	str	r0, [r2, #4]
	ldr r0, _0800B320
	ldr	r2, [r0, #0]
	cmp	r2, #0
	beq _0800B310
	cmp	r1, #0
	bne _0800B310
	str	r1, [r0, #0]
_0800B310:
	str	r1, [r0, #0]
	movs	r0, #0
	cmp	r3, #0
	bne _0800B31C
	cmp	r1, #0
	beq _0800B31E
_0800B31C:
	movs	r0, #1
_0800B31E:
	bx	lr
_0800B320: .4byte 0x300129c
.endif
