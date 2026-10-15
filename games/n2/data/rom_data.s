@ Crash Bandicoot 2: N-Tranced (Europe): everything after the code, from
@ 0x0805BEA8 to the end of the ROM, still incbin'd from the original
@ (docs/n_tranced.md).

	.section .rodata

	.incbin "baserom_n2.gba", 0x5BEA8
