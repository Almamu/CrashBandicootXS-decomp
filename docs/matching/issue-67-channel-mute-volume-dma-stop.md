# Issue #67: `0x08038FD0`-`0x08039214` (audio, channel mute/volume + DMA stop)

Matches 5 of the 6 functions `docs/status/audio.md`'s `0x08038FD0` entry
previously listed as left raw - `sub_8038FD0`/`sub_8039064`/`sub_80390F8`
(the per-channel mute/volume-set family, previously written up as a
"confirmed many-register loop-allocation ceiling... no C rephrasing tried
reproduced that") and `sub_8039198`/`sub_80391E8` (the Direct Sound A/
Timer0 stop pair, previously "not attempted"). `sub_8039214` (a word-wrap
text/console-tile renderer) is still left raw - see below.

## The "confirmed ceiling" wasn't actually a ceiling

The earlier writeup's evidence was real (an isolated compile of the
obvious, idiomatic C - caching `gUnknown_03001630->channels[curChannelIdx]`
into a named local before the loop - always hoists it into a callee-saved
register, since that's exactly what a loop-invariant deserves), but the
conclusion it supported was too broad. The ROM's own codegen for these
three functions never spills anything past `r0`-`r3` (the loop-shaped two
don't even `push {r4-r7}`; the volume-setter pair only pushes `r4` for the
clamped volume parameter itself), which only happens when the *source*
never gives gcc-2.9 a single shared value to hoist in the first place -
i.e. when every occurrence of the channel chase is its own fresh
expression. A macro (`GAX_CHAN()`, re-expanded textually at each use, see
`src/audio/gax_channel_mute_volume.c`'s header comment) reproduces this
exactly: written this way, all three functions matched byte-for-byte in
isolation on the first real attempt.

## `sub_8039064`'s register-order gap - isolated compile is not proof

`sub_8039064` looked like a clean match in isolated `.s` output (all three
functions did), but the earlier real `make compare` (following
`docs/workflow.md` step 3's warning to the letter) caught a genuine 3-byte
regression in the linked ROM that the isolated compile's text comparison
missed by eye: `chan + (i << 2)` (and the single-index equivalent,
`chan + (idx << 2)`) - reached inside a nested pointer-cast expression -
always canonicalizes to "pointer operand first" (`adds r0, r1, r0`)
regardless of the C-level operand order, while the ROM computes it the
other way (`adds r0, r0, r1`, with the shifted index first and the channel
pointer second). No plain-C rephrasing (including swapping the addition's
operand order at the source level, which gcc-2.9 flattens away during its
own pointer-arithmetic canonicalization) reproduced the ROM's exact
instruction. The fix: pin the channel pointer to `r1` and the shifted
index to `r0` via `register ... asm("rN")`, then force the literal
`adds r0, r0, r1` byte sequence via a tiny extended-asm block with real
input/output operands (not a bare `asm("...")` string, which would let
gcc treat the pinned variables' defining loads as dead and delete them -
confirmed by watching an early attempt do exactly that). One more gotcha
surfaced getting *that* block itself right: gcc-2.9's inline-asm here only
accepts the pre-UAL Thumb mnemonic (`add r0, r0, r1`), not `adds` (`as`
rejects `adds` as "instruction not supported in Thumb16 mode" in this
context, unlike the plain-mnemonic register-add form GAS does accept) -
both call sites use `add`, not `adds`, for this reason.

## Direct Sound A/Timer0 stop pair

`sub_8039198` and `sub_80391E8` are the load-bearing counterpart to
`sub_8038B68`'s play-start follow-up (`gax_playback_ticker.c`): they
disable the current channel's "active" flag (`chan->4`'s `+0x1a` byte,
same object `gax_channel_note_cut.c`'s `self` operates on), reset
`GaxPlayerState::state` to 0, disable `SOUNDCNT_X`, and re-arm/disarm
`DMA1CNT_H` back to its "off" value (`0x0640`, no bit 15) using the exact
same real-hardware settle-delay quirk as `sub_80384DC`
(`gax_hw_reset.c`) - the `.byte 0x1b, 0x1c` / `mov r8,r8`x3 sequence that
can't be written as plain `adds r3, r3, #0` text. `sub_80391E8` is a
smaller, parameterized sibling (`REG_ADDR_DMA1CNT_H + dmaIdx*12` selects
DMA1/2/3's own `CNT_H`, matching the fixed 0xc-byte spacing between DMA
blocks) that `sub_8039198` doesn't actually call - it duplicates the DMA1
instructions inline instead, matching the ROM's own layout even though it
reads slightly redundant.

Landing `sub_8039198` byte-exact needed the same "reused zero across two
stores" idiom already seen elsewhere in this codebase
(`sub_803A104`/`gax_channel_init.c`'s "two named zero temps" note is the
mirror case: here a *single* `u32 zero = 0;` local has to be reused for
both the `strb` and the later `state = 0` store, or gcc re-materializes a
redundant `mov r1, #0` for the second one), plus one `register u8 *inner
asm("r0")` pin on the dereferenced `chan->4` pointer so the value/pointer
operand order in the `strb` matches the ROM's `strb r1, [r0, #0x1a]`
rather than the reversed pairing plain C produces by default.

## Left raw

- `sub_8039214` - a word-wrap text/console-tile renderer called by the
  already-matched `sub_80392E0` (`gax_fatal_error.c`). Fully read and
  understood (a per-character loop computing a VRAM tilemap destination
  from column/row arguments, word-wrap lookahead up to 29 columns, and a
  handful of ASCII punctuation remaps before writing each glyph index),
  but not attempted as a C reconstruction this pass - genuinely complex
  control flow (nested lookahead loop with two different register-reuse
  patterns for the same "current character" value), deprioritized in
  favor of landing the five matches above solidly. A reasonable next
  target for whoever picks this up.

See `docs/status/audio.md` for the updated matched/left-raw summary and
`tools/report_units.py`'s `UNITS` table for the exact current file
boundaries.
