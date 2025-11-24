.include "asm/macros.inc"

.syntax unified
.arm

	thumb_func_start sub_80005D0
sub_80005D0: @ 0x080005D0
	ldr r1, _080005D8 @ =0x04000208
	movs r0, #0
	strh r0, [r1]
	bx lr
	.align 2, 0
_080005D8: .4byte 0x04000208
