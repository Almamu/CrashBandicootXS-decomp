@ libagbsyscall: the GBA BIOS SWI wrappers (the names pokeemerald and the
@ other GBA decomps use). Each is `svc #N; bx lr`; VBlankIntrWait zeroes r2
@ first. They are in alphabetical order in the ROM, as a linker pulls them
@ out of a libagbsyscall archive. Hand-written asm, never C: their range is
@ HANDWRITTEN in tools/report_units.py. Prototypes: <agb_syscall.h>.
@
@ SWI numbers: BgAffineSet 0xE, CpuFastSet 0xC, CpuSet 0xB,
@ LZ77UnCompVram 0x12, ObjAffineSet 0xF, RLUnCompVram 0x15, Sqrt 8,
@ VBlankIntrWait 5.

	.text
	.code 16

	.align 2, 0
	.global BgAffineSet
	.type BgAffineSet, %function
	.thumb_func
BgAffineSet:
	svc #0xe
	bx lr
	.size BgAffineSet, .-BgAffineSet
	.align 2, 0
	.global CpuFastSet
	.type CpuFastSet, %function
	.thumb_func
CpuFastSet:
	svc #0xc
	bx lr
	.size CpuFastSet, .-CpuFastSet
	.align 2, 0
	.global CpuSet
	.type CpuSet, %function
	.thumb_func
CpuSet:
	svc #0xb
	bx lr
	.size CpuSet, .-CpuSet
	.align 2, 0
	.global LZ77UnCompVram
	.type LZ77UnCompVram, %function
	.thumb_func
LZ77UnCompVram:
	svc #0x12
	bx lr
	.size LZ77UnCompVram, .-LZ77UnCompVram
	.align 2, 0
	.global ObjAffineSet
	.type ObjAffineSet, %function
	.thumb_func
ObjAffineSet:
	svc #0xf
	bx lr
	.size ObjAffineSet, .-ObjAffineSet
	.align 2, 0
	.global RLUnCompVram
	.type RLUnCompVram, %function
	.thumb_func
RLUnCompVram:
	svc #0x15
	bx lr
	.size RLUnCompVram, .-RLUnCompVram
	.align 2, 0
	.global Sqrt
	.type Sqrt, %function
	.thumb_func
Sqrt:
	svc #8
	bx lr
	.size Sqrt, .-Sqrt
	.align 2, 0
	.global VBlankIntrWait
	.type VBlankIntrWait, %function
	.thumb_func
VBlankIntrWait:
	movs r2, #0
	svc #5
	bx lr
	.size VBlankIntrWait, .-VBlankIntrWait
	.align 2, 0 @ pad with zeros, not the assembler's nops
