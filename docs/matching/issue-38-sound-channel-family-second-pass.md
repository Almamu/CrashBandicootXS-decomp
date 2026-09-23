# Issue #38 second follow-up: closing sub_8024708 for real, parking
# sub_8024590 as NAKED

A further pass over the two functions
[docs/matching/issue-38-sound-channel-family.md](./issue-38-sound-channel-family.md)
left as `NON_MATCHING` C reconstructions in `src/system/game_loop37.c` -
`sub_8024590` and `sub_8024708`. Both had a single documented residual
gap each; this pass found the actual fix for `sub_8024708` (now real,
matched C) and, while chasing `sub_8024590`'s documented gap, found the
prior pass's "every field/branch/call confirmed" claim was itself
incomplete - isolated per-function compiles are only a diagnostic (see
docs/workflow.md step 3), and re-diffing the whole function against the
ROM surfaced two more genuine gaps beyond the one gap the prior write-up
described, on top of a third, unfixable one.

## Matched: `sub_8024708`

The VRAM-bank-toggling tile-asset streamer + `gUnknown_03001314`
shadow-byte rebuild + palette DMA + `REG_DISPCNT` writer. Two
gotchas, on top of the `self`/`asset` register pins the first pass
already found:

- The `toggle != 0` branch's `asset + 0x200` scratch-offset computation
  needs to land in r2, not gcc's own natural choice of r1 (which happens
  to already match the *other*, `toggle == 0` branch's identical-shaped
  computation) - the documented gap from the first pass. Fixed with an
  `asm volatile("mov r2, #0x80\n\tlsl r2, r2, #2\n\tadd %0, %1, r2" :
  "=r"(addr) : "r"(asset) : "r2")` anchor, the same "hardcode the scratch
  register, let the output land wherever" idiom `hud_icon_widget_8a78.c`
  uses.
- The `gUnknown_03001314` shadow-byte rebuild (`& ~0x10 | bit`) was not
  actually confirmed correct as the first pass's write-up claimed: gcc's
  front end always schedules the `& ~0x10` mask/byte-read pair *before*
  the toggle-bit `& 1 << 4` shift-and-mask when both are written as
  independent statements, but the ROM computes the shifted toggle bit
  first. Fixing the order took two steps:
  1. An `asm volatile("" ::: "memory")` ordering barrier right after the
     toggle-bit shift forces it to materialize before the scheduler is
     free to move the mask/byte-read pair ahead of it.
  2. The barrier alone widens the shifted toggle-bit value's tracked
     range just enough that the final `orr` picks up an extra defensive
     `lsl #24; lsr #24` truncation pair the ROM doesn't have. Avoided by
     writing the AND as `bit4 & toggleByte` (not `toggleByte & bit4`) -
     a source-order swap that happens to pick the same destination
     register (r1, not r5) the ROM's own `ands r1, r5` uses, sidestepping
     whatever value-range analysis triggered the extra truncation.

Full clean `make compare` confirms byte-exact.

## Parked as NAKED: `sub_8024590`

Starts/re-selects a sound cue via `sub_8001B54`, then either plays its
secondary sfx immediately (if the channel already reports the requested
id) or busy-polls `sub_8001AB8` until it does, and either way ORs bit 7
into `field_08`'s low byte for a `sub_800132C` fade-start call.

Re-diffing the whole function against the ROM (not just the one branch
the first pass's write-up focused on) found three distinct gaps, not
one:

1. **Fixed for real**: the `field_08 | -0x80` redundant register-copy
   step, done twice (once per branch). The ROM materializes `-0x80` into
   r2 then copies it to r1 before the OR (`movs r2,#0x80; rsbs r2,r2,#0;
   adds r1,r2,#0; orrs r0,r1`); a plain `register s32 val asm("r1") =
   mask;` copy always got optimized away. Fixed with the same
   `asm volatile("" : "=r"(v) : "0"(expr))` forced-same-register-move
   idiom `settings_menu13.c` documents - `register s32 val asm("r1");
   asm volatile("" : "=r"(val) : "0"(mask));` forces the actual `mov`
   into r1 the ROM's own reuse of a free register produces.
2. **Fixed for real**: the very first `self->items[idx]` pointer load
   (fed straight to `sub_8001B54` as `item->field_14`) landed in r2 in
   this reconstruction but r1 in the ROM. The prior pass's write-up never
   flagged this because it never re-diffed this part of the function.
   Fixed by not naming it - `sub_8001B54(audio, self->items[idx]->field_14);`
   inline, instead of assigning through the same `item` local the rest of
   the function reloads - the "differently-named pointer variable avoids
   reuse" gotcha `settings_menu13.c` also documents (a shared register
   for a variable's whole lexical lifetime vs. a fresh one per
   transient use).
3. **Unfixable, confirmed toolchain bug**: the ROM's busy-poll loop
   (`playing != item->field_14`, entered when the channel doesn't
   already report the requested id) needs `push {r4, r5, r6, r7, lr}` -
   it caches `&gUnknown_030012BC` in r7 and a copy of the item
   byte-offset in r6 across the loop. But this compiler's
   `register T x asm("r7")` never adds an inline-asm-clobbered - or even
   a genuinely written-and-read - r7 to the function's own push/pop list.
   This is the same confirmed, extensively-precedented toolchain gap
   documented at length for `sub_8022D50` (`src/system/game_loop40.c`)
   and `LoadGraphicsPackage` (`src/graphics/graphics_package_1e578.c`,
   itself still `NON_MATCHING` for exactly this reason despite a fully
   pinned `register u16 *src asm("r7")` used throughout the function) -
   and every other `asm("r7")` call-out project-wide (`grep -rn
   'asm("r7")' src/` turns up over a dozen). No C-level technique
   (explicit pin, output operand instead of clobber, increasing register
   pressure to coax the natural allocator into picking r7 on its own)
   gets around it.

Given (1) and (2) are real, confirmed fixes and (3) is a hard toolchain
wall, this function is transcribed as `NAKED` straight from the
confirmed-correct ROM disassembly (`asm/code_3_2_17_24590.s`, now
deleted - its bytes are reproduced verbatim in the NAKED body) rather
than left `NON_MATCHING`, since the whole function is now fully
understood and NAKED gets the real, correct bytes in-tree. Per this
project's policy, NAKED means **parked, not matched** - tracked as such
in `docs/status/game_loop.md` and `tools/report_units.py`, and this pass
doesn't claim it toward closing issue #38.

`sub_8024590`/`sub_8024640`/`sub_80246D8`/`sub_8024708` are now all one
contiguous span in `src/system/game_loop37.c`'s own object
(`src/system/game_loop37.o`), so the two single-function raw asm files
that used to flank the matched middle (`asm/code_3_2_17_24590.s`,
`asm/code_3_2_17_24708.s`) are deleted and `ldscript.txt`/
`tools/report_units.py` updated to match - the same "widen the unit,
delete the orphaned raw `.s`" pattern this project already uses whenever
a flanking parked function gets fully matched or NAKED-transcribed (see
e.g. the `sub_8024344` entry, issue-38-sound-channel-family.md).

See [docs/status/game_loop.md](../status/game_loop.md) for the running
matched/parked/raw lists this updates.
