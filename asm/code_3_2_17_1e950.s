.include "asm/macros.inc"

.syntax unified
.arm

@ sub_801E950: not yet matched (NON_MATCHING) - see
@ src/graphics/graphics_package_1e8f8.c, guarded by #if NON_MATCHING - this
@ raw version is only assembled for the default (matching) build. See
@ docs/matching/issue-30-graphics-loading.md.
.if NON_MATCHING == 0
	thumb_func_start sub_801E950
sub_801E950: @ 0x0801E950
	movs r2, #3
	ands r1, r2
	lsls r1, r1, #2
	movs r2, #0xd
	rsbs r2, r2, #0
	ldrb r3, [r0, #0x15]
	ands r2, r3
	orrs r2, r1
	strb r2, [r0, #0x15]
	bx lr
.endif
