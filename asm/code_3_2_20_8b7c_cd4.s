.include "asm/macros.inc"

.syntax unified
.arm

@ AllocVramTileBlock: not yet matched (NON_MATCHING) - see
@ src/graphics/sprite_frame_queue.c, guarded by #if NON_MATCHING - this
@ raw version is only assembled for the default (matching) build.
@ Fully understood (a next-fit search over the OBJ-tile VRAM free-block
@ list, the same strategy mem_alloc uses over its own free list) and
@ every operation/field/register in the isolated compile matches the
@ ROM exactly except the search loop's entry shape: the ROM checks the
@ starting rover once on its own (a differently-registered copy of the
@ free+size check, `bge`/`blt` polarity) before falling into the shared
@ advance/re-check block, but this compiler's cross-jump/tail-merging
@ pass at -O2 collapses every C phrasing tried back down to one shared
@ check block reached via a leading unconditional branch instead - see
@ that function's own comment for the full list of phrasings tried.
.if NON_MATCHING == 0
	thumb_func_start AllocVramTileBlock
AllocVramTileBlock: @ 0x08028CD4
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	ldr r0, _08028D14 @ =gUnknown_03001338
	ldr r1, [r0]
	ldr r2, [r1, #0xc]
	adds r3, r1, #0
	ldrh r1, [r3, #6]
	adds r6, r0, #0
	cmp r1, #0
	bne _08028CEE
	ldrh r0, [r3, #4]
	cmp r0, r4
	bge _08028D00
_08028CEE:
	cmp r3, r2
	beq _08028D10
	ldr r3, [r3, #8]
	ldrh r0, [r3, #6]
	cmp r0, #0
	bne _08028CEE
	ldrh r1, [r3, #4]
	cmp r1, r4
	blt _08028CEE
_08028D00:
	ldrh r0, [r3, #4]
	subs r5, r0, r4
	cmp r5, #0
	beq _08028D38
	ldr r1, _08028D18 @ =gUnknown_0300133C
	ldr r2, [r1]
	cmp r2, #0
	bne _08028D1C
_08028D10:
	movs r0, #0
	b _08028D5A
	.align 2, 0
_08028D14: .4byte gUnknown_03001338
_08028D18: .4byte gUnknown_0300133C
_08028D1C:
	ldr r0, [r2, #8]
	str r0, [r1]
	movs r1, #0
	strh r5, [r2, #4]
	ldr r0, [r3]
	adds r0, r0, r4
	str r0, [r2]
	strh r1, [r2, #6]
	str r3, [r2, #0xc]
	ldr r0, [r3, #8]
	str r0, [r2, #8]
	str r2, [r0, #0xc]
	str r2, [r3, #8]
	strh r4, [r3, #4]
_08028D38:
	movs r0, #2
	strh r0, [r3, #6]
	ldr r0, [r3, #8]
	str r0, [r6]
	ldr r0, [r3]
	ldr r1, _08028D60 @ =0xF9FF0000
	adds r0, r0, r1
	lsrs r0, r0, #5
	ldr r1, _08028D64 @ =gUnknown_03001340
	ldr r1, [r1]
	adds r1, r1, r0
	ldr r0, _08028D68 @ =gUnknown_03001320
	ldr r0, [r0]
	subs r0, r3, r0
	asrs r0, r0, #4
	strb r0, [r1]
	ldr r0, [r3]
_08028D5A:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08028D60: .4byte 0xF9FF0000
_08028D64: .4byte gUnknown_03001340
_08028D68: .4byte gUnknown_03001320
.endif
