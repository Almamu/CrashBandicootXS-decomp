.include "asm/macros.inc"

.syntax unified
.arm

	arm_func_start start
start: @ 0x08000000
	b init_vector
	.include "asm/rom_header.inc"

init_vector:
	mov r0, #0x12
	msr cpsr_fc, r0
	ldr sp, _08000128 @ =gUnknown_03007FA0
	mov r0, #0x1f
	msr cpsr_fc, r0
	ldr sp, _08000120 @ =gUnknown_03007F00
	mov r0, #0xff
	svc #0x10000
	ldr sp, _08000124 @ =iwram_end
	ldr r2, _08000138 @ =0x040000D4
	ldr r0, _0800012C @ =gStaticData_087E55E4
	str r0, [r2]
	mov r0, #0x3000000
	str r0, [r2, #4]
	ldr r1, _08000130 @ =gUnknown_030009E8
	ldr r0, _08000134 @ =IntrMain_Buffer
	sub r0, r1, r0
	mov r1, #-0x7c000000
	orr r0, r1, r0, asr #2
	str r0, [r2, #8]
	ldr r1, _0800013C @ =AgbMain
	mov lr, pc
	bx r1
	b init_vector
	.align 2, 0
_08000120: .4byte gUnknown_03007F00
_08000124: .4byte iwram_end
_08000128: .4byte gUnknown_03007FA0
_0800012C: .4byte gStaticData_087E55E4
_08000130: .4byte gUnknown_030009E8
_08000134: .4byte IntrMain_Buffer
_08000138: .4byte 0x040000D4
_0800013C: .4byte AgbMain
