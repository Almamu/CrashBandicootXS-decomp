# Issue #68: `sub_8039B44`/`sub_8039E50` (audio)

The last raw pair from issue #68's `0x08039818`-`0x0803A944` chunk
(`docs/matching/issue-68-0x08039818-audio.md`'s "Left raw" section) -
one logical GAX2 per-channel note-trigger routine that the ROM's own
compiler split across two disassembly labels, glued together by the
same manual return-address-trampoline idiom already documented for
`sub_803A278`/`sub_803A2C8`/`sub_803A324`/`sub_803A5A8`
(`src/audio/gax_unknownc_play.c`, see
`docs/matching/issue-68-0x08039818-audio.md`).

## What it does

`self` (r6) is the channel object shared with the rest of this cluster
(`sub_8039818`/`sub_8039FFC`/`sub_803A104`/...) - `self+0x2a` is the
same signed-halfword "current note" field `sub_803A104` arms with the
`0x8AD0` "no note" sentinel. `info` (r4) is the shared handler object.
Called from the matched-but-NAKED `sub_80395A4`/`sub_803A158` (both
only when `self+0xc == 0`) as `sub_8039B44(self, info, arg1, arg2,
info->0->0x18, flag)`.

`sub_8039B44`'s half bails out early (returns 0) when `self+0x3c` (the
bound instrument table pointer) is NULL, when `self+0x2a` still holds
the `0x8AD0` sentinel, when `self+0x10` (voice-table row index) is out
of the 0-3 range, or when the instrument row's own flag byte is 0.
Otherwise it derives a base pitch from `self+0x2a`/`+0x2e`/`+0x21`/
`+0x26`/`+0x11` plus the instrument row's signed transpose byte, clamps
it into `gStaticData_085A62DC`'s period-lookup table (capped at
`0xEF3`), derives a per-voice volume by chaining `self+0x16`/`+0x17`/
`+0x15`/`+0x18`/`info->0->0x18->8` multiplies (each `0xff`-sentineled
to "skip"), calls `sub_8037ECC` (the 64-bit-division-backed pitch/
period helper, `src/util/math_div64_util.c`) on the result, then builds
a stack work-item and forwards it through `sub_800014C`.

The remainder (from ROM label `0x08039CDE` onward, i.e. `sub_8039E50`'s
half) loops the instrument's per-row envelope/pan table
(`gStaticData_0803A818`-relative row math against `self+0x3c`) while
`self+0x11` stays positive, updating `gUnknown_03001630->0x44`'s pan/
volume output halfword each iteration via one of several `self+0xd`/
`self+0x12`/`self+0x13`-gated paths, calling `sub_8037F3C` once per row
when `self+0xd` is set, and finally re-arms the `0x8AD0` "no note"
sentinel into `self+0x2a` (clearing `self+0x2c`) once the loop's row
count (`self->4->4`, a halfword) is exhausted, returning 1.

Voice/instrument object shape isn't modeled yet (same situation as the
rest of this GAX2 cluster) - kept as raw offsets throughout, matching
the neighboring functions in this directory.

## Why NAKED, not real C

This is a structural reason, not a register-allocation one (unlike most
of this cluster's other parked entries): the ROM's own compiler split
this single logical function into two disassembly labels,
`sub_8039B44`/`sub_8039E50`, using the same manual
return-address-trampoline idiom already documented for
`sub_803A278`/`sub_803A2C8`/`sub_803A324`/`sub_803A5A8` - `mov r2, pc;
adds r2, #5; mov lr, r2; bx r1` computes a Thumb-tagged return address
by hand and jumps through `r1` (`gUnknown_03001630`'s own `+0x44`
function-pointer slot, an interworked callback) instead of a normal
`bl`, since ARMv4T Thumb has no `blx reg`. That trampoline's return
address lands exactly at `sub_8039E50`'s first instruction - a `nop`
(`mov r8, r8`) alignment pad, the same tell already seen at
`sub_803A2C8`'s landing into `sub_803A318` - so the two ROM labels are
one physical function, not two independently callable ones. This idiom
itself is not expressible in portable C at all, regardless of register
pressure, so no real-C attempt was made for the trampoline sequence
specifically - and since the trampoline sits in the *middle* of the
function (not at entry/exit like the `sub_803A2C8` family), splitting
the surrounding logic into "the part before" and "the part after" as
separate real-C functions isn't possible either without breaking the
one-`bx`-lands-here-via-hand-computed-PC-offset relationship between
the two halves.

`sub_8039E50` keeps its own `.thumb_func`/`.global` label pair purely
so its ROM address still carries its name for anyone disassembling the
object (it is never itself called from anywhere in the ROM - checked
every `asm/*.s`/`expected/*.s`/`src/*` file for a `bl sub_8039E50`/
`.4byte sub_8039E50` reference, none exists), not as a second callable
C function - matching this project's established NAKED-fused-pair
policy.

## Transcription method

Mechanical, byte-verified transcription of the ROM's own instructions:
translated from the disassembler's unified syntax to this project's
established NAKED plain/divided syntax (`adds`->`add`, `movs`->`mov`,
`lsls`/`lsrs`/`asrs`->`lsl`/`lsr`/`asr`, `muls`->`mul`, `mvns`->`mvn`),
with the original's `_08XXXXXX:` labels renumbered to GNU-as local
numeric labels (`N:`, referenced `Nf`/`Nb`) per
`docs/matching/issue-4-sio-settings-sync.md`'s convention. Given this
function's size (~380 real instructions, this project's second-largest
NAKED transcription after that same doc's `sub_8002114`), a small
scratch Python script did the mechanical label-renumbering and
mnemonic translation instead of doing it by hand, specifically to avoid
the transcription-typo risk that scale invites (the same reasoning
`sub_8002114`'s writeup gives for its own script).

**Verification beyond the usual full clean `make compare`:** the
isolated compiled object's `.text` section was also byte-compared
directly against the *original* `asm/code_3_2_20e_9b44.s` reassembled
standalone (`arm-none-eabi-objcopy -O binary`, `cmp` byte-for-byte,
both producing exactly 1004 bytes) before ever touching
`ldscript.txt`/`tools/report_units.py` - catching any transcription
mistake immediately, independent of and prior to the full-ROM linked
build. The full clean rebuild (`rm -rf build && make NON_MATCHING=1
report`, then `rm -rf build crashbandicootxs.elf crashbandicootxs.gba
crashbandicootxs.map && make compare`) prints `La suma coincide`.

## Status

**Parked, not matched** - byte-correct NAKED asm, not real decompiled
C, per this project's NAKED-transcription policy. Both functions live
in the new `src/audio/gax_note_trigger.c`. This finishes issue #68's
last raw pair; every function in `docs/matching/issue-68-0x08039818-
audio.md`'s original `0x08039818`-`0x0803A944` range is now either
matched or parked (none left completely raw). Issue #68 stays open,
though, since 12 of the chunk's 21 functions are NAKED-parked rather
than matched - see `docs/status/audio.md` for the current matched/
parked breakdown.
