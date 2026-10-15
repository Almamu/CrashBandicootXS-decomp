@ IntrMain: the interrupt dispatcher, the first thing in the IWRAM image
@ (runs at 0x03000000, stored in ROM at 0x087E55E4; crt0 copies the image
@ and irq.cpp's IrqSetup points INTR_VECTOR here). Hand-written ARM (the
@ AGB SDK crt0 style, one branch per IRQ bit, no nesting), so it is
@ excluded from progress like crt0 (tools/report_units.py HANDWRITTEN).
@
@ It acknowledges the lowest pending, enabled interrupt in REG_IF and jumps
@ to its handler in gIntrTable (irq.cpp's handler table, one word per
@ IRQ bit). A VBlank also sets bit 0 of the BIOS IntrCheck flags at
@ 0x03FFFFF8 (0x04000000 - 8) for VBlankIntrWait. A Game Pak interrupt
@ (cartridge pulled) hangs on the last test instead of being dispatched.

.include "asm/macros.inc"

.syntax unified
.arm

	.text

	arm_func_start IntrMain
IntrMain: @ 0x03000000
	.global IntrMain_Buffer
IntrMain_Buffer:
	mov r12, #0x04000000		@ REG_BASE
	add r3, r12, #0x200		@ &REG_IE
	ldr r2, [r3]			@ REG_IE | REG_IF << 16
	and r1, r2, r2, lsr #16		@ pending & enabled
	mov r2, #0			@ handler table offset
	ands r0, r1, #0x1		@ VBlank
	strhne r0, [r12, #-8]		@ BIOS IntrCheck (0x03FFFFF8)
	bne .Lack
	add r2, r2, #4
	ands r0, r1, #0x2		@ HBlank
	bne .Lack
	add r2, r2, #4
	ands r0, r1, #0x4		@ VCount
	bne .Lack
	add r2, r2, #4
	ands r0, r1, #0x8		@ Timer 0
	bne .Lack
	add r2, r2, #4
	ands r0, r1, #0x10		@ Timer 1
	bne .Lack
	add r2, r2, #4
	ands r0, r1, #0x20		@ Timer 2
	bne .Lack
	add r2, r2, #4
	ands r0, r1, #0x40		@ Timer 3
	bne .Lack
	add r2, r2, #4
	ands r0, r1, #0x80		@ Serial
	bne .Lack
	add r2, r2, #4
	ands r0, r1, #0x100		@ DMA 0
	bne .Lack
	add r2, r2, #4
	ands r0, r1, #0x200		@ DMA 1
	bne .Lack
	add r2, r2, #4
	ands r0, r1, #0x400		@ DMA 2
	bne .Lack
	add r2, r2, #4
	ands r0, r1, #0x800		@ DMA 3
	bne .Lack
	add r2, r2, #4
	ands r0, r1, #0x1000		@ Keypad
	bne .Lack
	add r2, r2, #4
	ands r0, r1, #0x2000		@ Game Pak: hang
.Lhang:
	bne .Lhang
.Lack:
	strh r0, [r3, #2]		@ acknowledge in REG_IF
	ldr r1, .Lhandlers
	add r1, r1, r2
	ldr r0, [r1]
	bx r0
.Lhandlers: .4byte gIntrTable
	arm_func_end IntrMain
