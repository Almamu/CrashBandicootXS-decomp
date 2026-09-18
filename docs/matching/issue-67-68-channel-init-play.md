# `sub_8039518`/`sub_80395A4`/`sub_8039658`: the Channel type's init_fn/play_fn (audio, issues #67/#68)

Closes the two remaining raw gaps `docs/matching/issue-67-0x08038538-audio.md`
left open for the GAX2_SoundHandler "Channel" type's function-pointer trio
(`docs/audio.md`'s per-type table: init_fn/unknown_fn/play_fn =
`0x08039519`/`0x080395A1`/`0x080395A5` - the unknown_fn, `nullsub_40`, was
already matched in `gax_sound_handler_channel.c`). Both are NAKED asm
transcriptions - byte-correct but not real decompiled C, tracked as parked,
not matched - so issue #67/#68 stay open regardless of this pass.

## `sub_8039518` (Channel type's init_fn) - `src/audio/gax_sound_handler_channel_init.c`

Resets the same kind of field set `sub_803A104`'s per-channel voice
constructor does (accumulator/envelope/priority/portamento defaults, plus
two extra pointer-walked byte zeroes at `+0x24`/`+0x25` that don't fit a
Thumb `strb` immediate offset), computes a fixed-point reciprocal
(`sub_8037A7C((s64)1 << 32, (s64)self->field_4's 0x2 halfword)` - a generic
64-bit software division helper, not GAX2-specific, see `docs/audio.md`)
into the 8-byte global `gUnknown_03001618`, then loops `self->8`'s
child-pointer array (count `self->0->0xc`) firing each child's own function
pointer through the `sub_803AD7C` trampoline - the same pattern
`sub_803A22C` (UnknownC type's init_fn) uses.

A real C reconstruction got every field-reset store byte-identical on its
own, including reproducing the ROM's `ldr rX,=0`/`ldr rX,=1` literal-pool
loads for the division call's first two arguments - previously undocumented
as solvable: passing them as one real `(s64)1 << 32` C constant (rather than
two separate `int` arguments, which is what `docs/matching/issue-67-0x08038538-audio.md`'s
original pass tried) makes gcc materialize the 64-bit literal via
`thumb_load_double_from_address`, landing on the exact same two-word
literal-pool load the ROM has. That closed the one gap the previous pass
flagged - but two *new*, more fundamental compiler limitations surfaced once
that was fixed:

1. `sub_8037A7C`'s second argument (the halfword field read through a
   pointer chain) has to sit in a register pinned to `r2` to match the ROM's
   register choreography around the call, but gcc 2.9's explicit-register-
   variable support always emits one extra defensive copy (`adds r6, r2,
   #0`) between computing a pinned register's value and consuming it at a
   call site, even when both already agree on the same physical register -
   confirmed by trying every combination of pinned/unpinned intermediates
   for both the temporary field-pointer and the halfword result.
2. The ROM groups all 4 of this function's literal-pool words (`0x8AD0`,
   `gUnknown_03001618`'s address, and the division call's `0` and `1`) into
   one pool sitting right after the `b` that skips over it - gcc's own
   pooling naturally reproduces this (it flushes every compiler-visible
   constant used by a function at the same point), but as soon as any
   instruction in that stretch becomes a hand-placed inline-asm anchor (per
   point 1), the anchor's own `ldr rX,=value` pseudo-ops pool separately
   from gcc's, splitting the 4 words across two pools and shifting every
   following instruction's address away from the ROM's.

Merging them back requires hand-placing the whole stretch from the first
pool-pinned constant onward as one block - at which point it's a mechanical,
byte-verified transcription of the ROM's own instructions, not an inferred
control-flow guess (every field offset and the loop bound were independently
understood first, the same as this project's other NAKED transcriptions).

## `sub_80395A4`/`sub_8039658` (Channel type's play_fn and its direct callee) - `src/audio/gax_sound_handler_channel_play.c`

`sub_80395A4` (`self` = this channel's own handler, `info` =
`*(void**)(self+8)`, the shared Info handler every channel's `children_ptr`
points at per `docs/audio.md`) first forwards its own `(arg1, chanArg)`
straight into `info`'s own play_fn slot (`info->0->0x8`, the same
three-function-pointer-per-type table this function itself is a member of)
via the `sub_803AD84` "call through r3" trampoline - the established
`sub_803AD84(addr, a1, a2, fn)` parameter order (see
`src/graphics/actor_part17.c`/`src/system/game_loop16.c`) landed the
function-pointer argument in `r3` with zero extra effort, and this part of
the reconstruction was byte-exact immediately. Then: if `info` armed a
"retrigger" flag (`info->0x1b`), clears this channel's own `field_0x3c`; if
`info` is armed at all (`info->0x1a`), advances this channel's own countdown
(`field_0x34`, a signed halfword) and fires `sub_8039658(self, info, 1)`
once it hits zero, or fires `sub_8039658(self, info, 0)` whenever
`info->0x18` is set and `info->0x1d` is set; drives a
`field_0x1f`/`field_0x1e` "note-cut countdown" pair (calling `sub_80398DC`
each time it's re-armed from `field_0x1e`); always runs `sub_8039AA4`; and
finally, only when `self->0xc == 0`, forwards into `sub_8039B44(self, info,
arg1, chanArg, info->0->0x18, 0)` and returns its low byte.

Every load/store/branch/call in the body above landed byte-identical in
isolation - the entire semantic reconstruction is solid. What resisted
matching was purely the 3-instruction parameter-homing sequence right after
the prologue (`self`/`arg1`/`chanArg` copied into `r4`/`r6`/`r7`, all three
needing to survive the first call): the ROM orders this `r4, r6, r7` (`self`
first), but this compiler's register allocator - unpinned, or pinned in
every combination of which one or two of the three get an explicit
`register T x asm("rN")` pin - only ever produces `r6, r7, r4` or `r7, r4,
r6`. `chanArg` can't be pinned to force the issue further: pinning it to
`r7` hits this toolchain's already-documented pin bug (an explicit
`register T x asm("r7")` compiles with no push/pop of `r7` at all - see
`hud_icon_widget_85c4.c`'s `InitHudIconWidgetA`/`B` write-up for the same
conclusion on an unrelated function), so only fully-unpinned allocation can
safely put a value in `r7` at all, and that path's own ordering choice never
matches the ROM's. Transcribed instruction-for-instruction instead.

`sub_8039658` is `sub_80395A4`'s direct callee - a per-note-event command
dispatcher. `self+0x40` is a rolling cursor into the current pattern-row
byte stream (`docs/audio.md`'s per-song pattern data); this decodes one
"packed row" record (a leading control byte whose top two bits select how
many of note/instrument/volume/effect follow) unless `self+0xe`/`self+0xf`
(a pending-rows/hold countdown pair) says to skip decoding this tick,
dispatches the decoded command byte (an effect-command index) through a
15-entry jump table (most cases just store a nibble into a channel field;
cases 6/14 also poke the shared `info` handler's own state), and always
re-primes `self+0x1a`/`0x28`/`0x34` to 0 up front and calls
`sub_8039818`/`sub_803985C` (per-channel note-cut dispatch and voice
trigger) unless the decoded effect index is exactly 3. `flag` (the caller's
`1`/`0`) selects between reading a *new* record from the pattern stream
(`flag == 0`) or replaying the *same* `self+0x50`/`0x51` "last command"
bytes again (`flag != 0`, the retrigger case). This one was never
attempted as plain C: its prologue alone (`push {r4-r7,lr}; mov r7,sb; mov
r6,r8; push {r6,r7}`) needs both `r8`/`sb` as genuine scratch across the
packed-row decode - the same many-register gcc-2.9 allocation ceiling
already documented throughout this ROM region for `sub_8038538`'s cluster
(`docs/status/audio.md`). Its 15-entry jump table is hand-placed with named
local labels (the same "hand-placed local labels shared across a single
literal pool" idea as `actor_part38b.c`'s `sub_80151C8` jump table) rather
than numbered ones, given how many branch targets this function has.

Both were transcribed instruction-for-instruction from the ROM disassembly
(translated from the disassembler's unified syntax to this project's
established NAKED plain/divided syntax), not inferred control-flow guesses -
every field offset, branch condition, and the jump table's case mapping were
independently understood first via the same reading that wrote the doc
comments above.

Object shape for both `self`/`info` isn't confidently modeled yet (same
situation as the other GAX2_SoundHandler functions in this cluster) - kept
as raw offsets throughout.

Full clean `make compare` verified `La suma coincide` after cutting all
three functions out of `asm/code_3_2_20e_9518.s`/`asm/code_3_2_20e_95a4.s`
(both files now empty and removed) and adding
`src/audio/gax_sound_handler_channel_init.o`/
`src/audio/gax_sound_handler_channel_play.o` to `ldscript.txt` in their
place.
