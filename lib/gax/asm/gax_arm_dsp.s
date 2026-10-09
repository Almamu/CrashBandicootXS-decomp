@ GAX2's ARM-mode DSP routines. Hand-written ARM in the original: GAX
@ 3.05A keeps the same routines (render/downmix, low-pass filter, reverb,
@ resampler, with the same "FILT"/"BART" tags) as assembly too
@ (docs/libraries.md, "GAX implementation notes"). They never run from
@ ROM: GAX2_init copies them into the player state (downmixCode,
@ echoCode, filterCode, mixCode) and the Thumb code enters the copies
@ through GAX_CALL_ARM (lib/gax/src/gax_internal.h), Shin'en's own
@ inline asm with a hand-computed return address. Each takes
@ a pointer to a work item in r0 and returns with `bx lr`. The filter and
@ echo routines keep state words inside their own code (pc-relative), so
@ each copy has its own state. Divided syntax.

.include "asm/macros.inc"

	.text
	.arm

	.align 2, 0
	@ 8 zero bytes nothing references
	.word 0, 0

@ gGaxArmDownmix(item): GaxMixFrame's 16-bit mix buffer to 8-bit output.
@ item = {s16 *src, s8 *dst, u32 samples, u32 gain}:
@ dst[i] = clamp((src[i] * gain) >> 10, -128, 127) for i < samples.
	arm_func_start gGaxArmDownmix
gGaxArmDownmix:
	stmfd sp!, {r5, r6}
	ldr r5, [r0, #12]		@ gain
	ldr r2, [r0, #8]		@ samples
	ldr r1, [r0, #4]		@ dst
	ldr r0, [r0]			@ src
	add r2, r2, r1			@ dst end
.Ldownmix_loop:
	ldrsh r6, [r0], #2
	mul r6, r5, r6
	mov r6, r6, asr #10
	cmn r6, #128
	mvnlt r6, #127			@ < -128: -128
	cmp r6, #127
	movgt r6, #127			@ > 127: 127
	strb r6, [r1], #1
	cmp r1, r2
	blt .Ldownmix_loop
	ldmfd sp!, {r5, r6}
	mov r0, lr
	bx r0
	arm_func_end gGaxArmDownmix

@ gGaxArmFilter(item): GaxMixerApplyFilter's low-pass filter, two
@ cascaded one-pole stages over the mix buffer in place.
@ item = {s16 *buf, u32 bytes, u32 amount (0x55 - filter), u32 channels};
@ coefficient = amount * 0x334 >> 8, every stage clamped to
@ +-(0x7F00 * channels). The two stages' outputs carry over to the next
@ call in .Lfilter_state.
	arm_func_start gGaxArmFilter
gGaxArmFilter:
	stmfd sp!, {r4-r11}
	ldr r1, [r0, #12]		@ channels
	ldr r7, [r0, #8]		@ amount
	ldr r2, [r0, #4]		@ bytes
	ldr r0, [r0]			@ buf
	adr r4, .Lfilter_state
	ldr r5, [r4, #4]		@ stage 1
	ldr r4, [r4]			@ stage 2
	mov r8, #0x334
	mul r7, r8, r7
	mov r7, r7, asr #8		@ coefficient
	mov r9, #0x7F
	mov r9, r9, lsl #8
	mul r9, r1, r9			@ clamp limit
.Lfilter_loop:
	ldrsh r8, [r0]
	mov r8, r8, lsl #8
	sub r6, r8, r5
	sub r6, r6, r4
	cmn r6, r9
	mvnlt r6, r9
	cmp r6, r9
	movgt r6, r9
	mul r10, r6, r7
	add r5, r5, r10, asr #8
	cmn r5, r9
	mvnlt r5, r9
	cmp r5, r9
	movgt r5, r9
	mul r10, r5, r7
	add r4, r4, r10, asr #8
	cmn r4, r9
	mvnlt r4, r9
	cmp r4, r9
	movgt r4, r9
	mov r10, r4, asr #8
	strh r10, [r0]
	add r0, r0, #2
	subs r2, r2, #2
	cmp r2, #0
	bgt .Lfilter_loop
	adr r0, .Lfilter_state
	str r4, [r0]
	str r5, [r0, #4]
	ldmfd sp!, {r4-r11}
	bx lr
.Lfilter_state:
	.word 0, 0
	.ascii "FILT"
	arm_func_end gGaxArmFilter

@ gGaxArmEcho(item): GaxMixerApplyEcho's echo, up to three taps of a
@ circular delay line, mixed into the buffer in place.
@ item = {u32 bytes, s16 *buf, s16 *echoBuf, u32 echoLen (bytes),
@ taps}; taps = three {delay (bytes), gain} pairs, a zero delay ending
@ the list after the first. Each output sample is also written to the
@ delay line at the running position kept in .Lecho_pos. The third tap's
@ gain is loaded but never multiplied in (its sample is added >> 4).
	arm_func_start gGaxArmEcho
gGaxArmEcho:
	stmfd sp!, {r4-r8}
	ldr r5, .Lecho_pos
	ldr r4, [r0]			@ bytes
	ldr r3, [r0, #12]		@ echoLen
	ldr r2, [r0, #16]		@ taps
	ldr r1, [r0, #8]		@ echoBuf
	ldr r0, [r0, #4]		@ buf
	add r4, r4, r0			@ buf end
.Lecho_loop:
	cmp r5, r3
	moveq r5, #0			@ wrap the delay-line position
	add r7, r5, r3
	ldr r8, [r2]
	sub r7, r7, r8
	cmp r7, r3
	subge r7, r7, r3
	ldrsh r7, [r1, r7]
	ldr r8, [r2, #4]
	muls r7, r8, r7
	mov r6, r7, asr #4		@ tap 1
	ldr r8, [r2, #8]
	cmp r8, #0
	beq .Lecho_mix
	add r7, r5, r3
	sub r7, r7, r8
	cmp r7, r3
	subge r7, r7, r3
	ldrsh r7, [r1, r7]
	ldr r8, [r2, #12]
	muls r7, r8, r7
	add r6, r6, r7, asr #4		@ tap 2
	ldr r8, [r2, #16]
	cmp r8, #0
	beq .Lecho_mix
	add r7, r5, r3
	sub r7, r7, r8
	cmp r7, r3
	subge r7, r7, r3
	ldrsh r7, [r1, r7]
	ldr r8, [r2, #20]
	add r6, r6, r7, asr #4		@ tap 3 (r8, its gain, unused)
.Lecho_mix:
	ldrsh r8, [r0]
	add r8, r8, r6
	strh r8, [r0]
	strh r8, [r1, r5]		@ into the delay line
	add r0, r0, #2
	add r5, r5, #2
	cmp r0, r4
	blt .Lecho_loop
	adr r0, .Lecho_pos
	str r5, [r0]
	ldmfd sp!, {r4-r8}
	bx lr
	.word 0				@ unused
.Lecho_pos:
	.word 0
	.ascii "BART"
	arm_func_end gGaxArmEcho

@ gGaxArmResample(item): GaxChannelMix's resampler, one channel's 8-bit
@ sample into the 16-bit mix buffer at a Q11 step.
@ item = {s8 *src, s16 *buf, s32 pos, s32 end, u32 frames, u32 done,
@ u32 volume, u32 step, u32 mode, s32 loopLen} (positions Q11);
@ out = buf + done, up to buf + frames; sample = src[pos >> 11] * volume
@ >> 8. Modes (the jump table): 0 stores (the first channel mixed),
@ 1 adds, 2 adds at half rate (each sample into two outputs, stepping
@ twice as far; only in the full copy, GaxPlayerState.fullResampler).
@ Each loop is entered at its test. When pos passes end, a loop with a
@ loopLen wraps by it; otherwise the routine returns. On return
@ item.pos and item.done are updated.
@
@ GaxChannelMix patches the copy before each call: the step instruction
@ of the store and add loops (`add r2, r2, r8` forward, `sub r2, r2, r8`
@ backward; their top halfwords 0xE082/0xE042) and the end test after
@ it (`blt` forward, `bgt` backward: 0xBAFF/0xCAFF). Those four
@ instructions carry the global labels.
	arm_func_start gGaxArmResample
gGaxArmResample:
	stmfd sp!, {r0, r4-r11}
	ldr r7, [r0, #24]		@ volume
	ldr r8, [r0, #28]		@ step
	ldr r1, [r0, #4]		@ buf
	ldr r2, [r0, #8]		@ pos
	ldr r3, [r0, #12]		@ end
	ldr r4, [r0, #16]
	add r4, r1, r4, lsl #1		@ output end: buf + frames
	ldr r5, [r0, #20]
	add r1, r1, r5, lsl #1		@ out: buf + done
	ldr r10, [r0, #32]		@ mode
	ldr r11, [r0, #36]		@ loopLen
	ldr r0, [r0]			@ src
	ldr r10, [pc, r10, lsl #2]
	add pc, pc, r10
.Lresample_modes:
	.word .Lresample_store_test - .Lresample_modes - 4
	.word .Lresample_add_test - .Lresample_modes - 4
	.word .Lresample_half_test - .Lresample_modes - 4

.Lresample_store_loop:			@ mode 0
	mov r6, r2, asr #11
	ldrsb r6, [r0, r6]
	mul r6, r7, r6
	mov r6, r6, asr #8
	strh r6, [r1], #2
	.global gGaxArmResampleStoreStep
gGaxArmResampleStoreStep:
	add r2, r2, r8
.Lresample_store_test:
	cmp r1, r4
	bge .Lresample_done
	cmp r2, r3
	.global gGaxArmResampleStoreEndTest
gGaxArmResampleStoreEndTest:
	blt .Lresample_store_loop
	cmp r11, #0
	beq .Lresample_done
	sub r2, r2, r11
	b .Lresample_store_loop

.Lresample_add_loop:			@ mode 1
	mov r6, r2, asr #11
	ldrsb r6, [r0, r6]
	mul r6, r7, r6
	mov r6, r6, asr #8
	ldrsh r9, [r1]
	add r6, r6, r9
	strh r6, [r1], #2
	.global gGaxArmResampleMixStep
gGaxArmResampleMixStep:
	add r2, r2, r8
.Lresample_add_test:
	cmp r1, r4
	bge .Lresample_done
	cmp r2, r3
	.global gGaxArmResampleMixEndTest
gGaxArmResampleMixEndTest:
	blt .Lresample_add_loop
	cmp r11, #0
	subne r2, r2, r11
	bne .Lresample_add_loop

.Lresample_done:
	ldmfd sp!, {r0, r4-r11}
	ldr r3, [r0, #4]
	sub r3, r1, r3
	mov r3, r3, lsr #1
	str r2, [r0, #8]		@ item.pos
	str r3, [r0, #20]		@ item.done = out - buf
	mov r0, lr
	bx r0

.Lresample_half_loop:			@ mode 2
	mov r6, r2, asr #11
	ldrsb r6, [r0, r6]
	mul r6, r7, r6
	mov r6, r6, asr #8
	mov r6, r6, lsl #16
	orr r6, r6, r6, lsr #16		@ the sample in both halfwords
	ldr r9, [r1]
	add r9, r6, r9
	str r9, [r1], #4
	add r2, r2, r8, lsl #1
.Lresample_half_test:
	cmp r2, r3
	cmplt r1, r4
	blt .Lresample_half_loop
	cmp r2, r3
	bgt .Lresample_half_back
	cmp r1, r4
	ble .Lresample_done
.Lresample_half_back:			@ overshot: back off one output and step
	sub r1, r1, #2
	sub r2, r2, r8
	b .Lresample_done
	arm_func_end gGaxArmResample

	.align 2, 0
