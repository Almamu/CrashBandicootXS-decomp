# Issue #37: 0x080234E8-0x0802425C - checkpoint/level-transition helpers

GitHub issue #37 (`decomp-chunk`, category `game_loop`) listed 25 raw
functions, generated against `asm/code_3_2_17.s` but actually living in
`asm/code_3_2_17_231cc.s` (the file gets split further with every
matching pass, so the issue's file pointer is a point-in-time snapshot
like everything else about it). This is the write-up for the work done
against that list.

**Naming note:** these files are numbered `game_loop10`/`game_loop11`
rather than `game_loop6`/`game_loop7` (which would have matched their
original creation order more naturally) because issue #12's parallel PR
independently claimed `game_loop6.c`/`game_loop7.c` first for unrelated
functions before this PR merged - resolved as a rename on merge to
avoid an add/add filename collision.

## What this cluster turned out to be

A grab-bag of small helpers on the same large "self" object already
used throughout the `game_loop2.c`/`game_loop3.c` family (no dedicated
struct here either, for the same reason those files give: most of its
fields are only ever touched by functions still raw elsewhere) -
camera-position setters, checkpoint/level-transition snapshot helpers
that stash and restore a `0x68`-byte block at `self+0xe4`, a packed
bitfield accessor pair at `self+0x14c`/`0x14d`, the `PlayCutscene`
mode-trampoline family, a DMA3/VRAM refresh pass gating on
`self+0x0 <= 0x1000` ("near start of level"), a `REG_BLDCNT`/
`REG_BLDALPHA` shadow-word rebuild (see `src/util/aabb.c`'s
`CommitBlendRegs`, which commits that same shadow to hardware), and the
level-end teardown/VRAM-flush tail. Sandwiched in the middle of all
that: two large functions (`PlayRoom`, ~300 instructions;
`RunRoom`, ~650 instructions, its own internal jump-table state
machine with cross-branch `goto`-style jumps) that read like the real
level-start dispatcher and its post-processing continuation - clearly
important, but not confidently understood branch-by-branch within this
pass's scope, so left completely untouched.

## Matched (24 functions, full clean `make compare` passing)

`src/system/game_loop10.c` (`SetGemPlatform`-`PackSaveData`, 15 fns):
`SetGemPlatform`/`SetBonusPlatform` (camera-position field setters),
`SetCrateGemPos` (two-word position setter), `RequestGemPath`/`RequestBonusRound`
(busy-flag setters gated on `IsInGemPath`/`IsInBonusRound`),
`RestoreCheckpoint`/`SetCheckpoint` (checkpoint snapshot restore/stash pair,
the latter also flushing two spans of the `gEntityFlags` bitmap
via the `CpuSet` wrapper), `EndGemPath` (progress-accumulate-or-reset
dispatcher), `PlayNewGameCutscene`/`PlayIntroCutscene`/`PlayBootCutscene` (the
`PlayCutscene` mode-trampoline family, one of them also playing a fixed
SFX), `ShowCompanyLogos` (allocates a `0x44c`-byte block and hands it to
`DestroyCompanyLogos`), `nullsub_24` (empty stub), `UnpackSaveData` (bitfield
unpacker, refreshing its own snapshot first), `PackSaveData` (its
packer inverse - see the update below, added after this doc's original
pass).

`src/system/game_loop11.c`: `GetLevelState` (lazy-allocates and returns
`gLevelStateSingleton`) - its own file since the still-raw
`PlayRoom`/`RunRoom` pair sits on both sides of it in ROM order.

`src/system/game_loop8.c`: `UpdateRoomFrame` (the DMA3/VRAM refresh pass).

`src/system/game_loop9.c` (`ClearRoomExit`-`ResetObjBuffers`, 5 fns):
`ClearRoomExit`/`RequestRoomExit`/`IsRoomExitRequested` (a boolean flag
clear/set/get trio on `gRoomExitRequested`), `ResumeRoomAfterPause` (level-end
teardown: DMA-copies the level's first palette word into `PLTT`,
clears it, then re-runs `UpdateRoomFrame`'s refresh pass and four
`display.c` state resets), `ResetObjBuffers` (the shared
vram-upload-cursor/OAM-shadow flush tail both `UpdateRoomFrame` and
`ResumeRoomAfterPause` end with).

### Gotchas worth recording

- **`SetCrateGemPos`/`SetCheckpoint`'s two-word field copies**: writing
  `dst->field0 = x; dst->field1 = y;` as two independent raw-offset
  stores makes this compiler compute two full addresses from scratch.
  The ROM computes the base pointer once and uses immediate-offset
  stores off it - reproduced by declaring a local `s32 *dst` pointer
  and indexing `dst[0]`/`dst[1]` instead of re-deriving the address
  each time (see also the pre-load-both-then-store-both ordering below,
  needed when the *source* is also a two-word read).
- **`SetCheckpoint`'s repeated `0x04000040` `CpuSet` control word**:
  a plain literal used identically in two nearby calls gets CSE'd into
  one shared register held live across both calls, unlike the ROM
  (which reloads it from its literal pool each time). Fixed with a
  fresh `register u32 ctrl asm("r2") = 0x04000040;` block scoped to
  each call individually - but the *placement* of that block mattered
  as much as the pin itself: declaring `ctrl` **before** the two
  pointer-offset arguments were computed made the compiler materialize
  r2 first (bumping the address computation onto r3 instead, wrong
  register entirely); declaring it **after** two `void *a = ...`/
  `void *b = ...` locals for the pointer arguments, immediately before
  the call, reproduced the ROM's exact "compute both addresses, then
  load the control word right before `bl`" order. This one is the
  reason `SetCheckpoint` isn't in the first commit of this branch's
  history - the isolated per-function compile looked identical to the
  ROM but the full-ROM `make compare` (as `docs/workflow.md` step 6
  warns) caught the real byte offset.
- **`RestoreCheckpoint`/`SetCheckpoint`'s "read-then-relocate-pointer" byte
  copies** (`self->0xa9 = self->0xd0`, `self->0xd0 = self->0xa9`):
  writing the assignment directly lets the compiler compute the
  destination address before reading the source in one case, or fuse a
  same-object offset into a `+4`-style relative access that doesn't
  match the ROM's independent address derivation in another. Fixed
  with an explicit `u8 tmp = *src; *dst = tmp;` two-step, forcing the
  read to happen (and the value to be captured) before the destination
  address is computed.
- **`ShowCompanyLogos`'s `nullsub_7` call**: the ROM keeps the freshly
  allocated block's pointer in r0 *across* the `bl nullsub_7` call and
  only moves it to a callee-saved register afterward - only possible
  because `nullsub_7` (now matched separately, in
  `src/frontend/language_select.c`) was compiled in the same translation
  unit as this function originally, so the compiler could prove it
  doesn't touch r0. Split across files, an ordinary C call must
  conservatively assume r0-r3 are clobbered and moves the pointer to
  r4 *before* the call - one instruction too early. Reproduced by
  spelling that one call as inline asm (`asm volatile("bl nullsub_7" :
  "+r"(tmp) :: "r1", "r2", "r3", "lr", "cc")`) that tells the compiler
  the pointer register survives, which is true here and lets the
  delayed move happen exactly where the ROM has it - the *next* call
  (`RunCompanyLogos`, a real, unrelated function) still forces the normal
  conservative move beforehand, so this isn't a general "keep values in
  r0 forever" trick, just an accurate description of this one no-op
  call's real effect.
- **`UnpackSaveData`'s byte/halfword extracts**: the ROM loads each raw
  byte/halfword into one register and computes the shifted result into
  a *different* one (`ldrb r1,[r5]` then `lsls r0,r1,#0x19`), rather
  than shifting in place - the same "freshly-loaded value and its
  transformed result in different registers" pattern documented
  elsewhere in `docs/matching.md`. Fixed with explicit `register`
  pins for the raw value (r1) and the shifted result (r0), reusing r5
  (the snapshot pointer) for the halfword load too, matching the ROM's
  register reuse exactly.
- **`UpdateRoomFrame`'s `_call_via_r1` reload pattern**: computing
  `gPlayer` and its `->table` field into plainly-named
  locals both times let the compiler pick whichever register was
  convenient rather than reloading through r0 the way the ROM does at
  both call sites (needed since the global's value can change as a
  side effect of the first `_call_via_r1` call). Fixed by pinning the
  reloaded `struct actor *` to r0 at each of the two call sites.

## Left raw - 2 functions

- **`PlayRoom`/`RunRoom`** (`asm/code_3_2_17_2375c.s`, ROM
  `0x0802375C`-`0x08024007`) - a ~300-instruction level-start
  dispatcher (allocates and initializes several HUD/counter widget
  objects via `OperatorNew`+`SetCtrlAnimSet`/`_call_via_r2`, dispatches on
  a 3-way record-type switch) feeding into a ~650-instruction
  continuation (`RunRoom`) built around a 6-case jump table with
  cross-branch jumps into a shared tail (`_08023BB8`/`_08023BBE`
  setting a state flag and jumping into the middle of a *different*
  branch's cleanup code, `_08023E72`). The overall shape (spawn a HUD
  widget set, run a per-frame update loop gated on `IsRoomExitRequested`,
  react to a completion signal from `RunPauseMenu`) is legible, but
  several callees (the `gStaticData_0816C8xx` tables' exact record
  shape, `AddPaletteCycle`'s 6-argument signature, `LoadRoom`,
  `InitPlayer`, `TickPaletteCycles`) aren't characterized precisely enough
  yet to commit to a byte-exact reconstruction of this size with
  confidence - left untouched rather than force a low-confidence
  match. A good next target once the HUD-effect-queue family
  (`AddPaletteCycle`/`TickPaletteCycles`, currently left raw per
  `tools/report_units.py`'s `hud` entries) is better understood.

See [docs/status/game_loop.md](../status/game_loop.md) for the running
matched/parked/raw lists this updates.

## Update: `EndBonusRound`/`SetCheckpointAtPlayer` matched

Both of this entry's two parked functions are now byte-exact matched
(full clean `make compare`: `crashbandicootxs.gba: La suma coincide`),
closing out the last of the original 25-function chunk. `asm/code_3_2_17_22bf0.s`
(which held only these two functions' raw bytes) is deleted; its
`ldscript.txt` line is removed.

- **`EndBonusRound`**: the earlier attempt cached all four of
  `self+0x70`/`0x6c`/`0x74`/`0xbc` behind pointer locals, which spilled
  into `r8`/`r9`/`sl`. The fix: `self+0x70`/`0x6c`/`0x74` all fit the
  Thumb `ldr`/`str` immediate range (0-124) and the ROM addresses them
  directly off `self` with no cached pointer at all - only
  `0xb4`/`0xb0`/`0xb8`/`0xd4`/`0xe0`/`0xbc` (all past that range)
  actually need an address computed into a local. Dropping the three
  unnecessary pointer locals brought the whole function down to the
  ROM's exact `r4`-`r7` register set, no spill.
- **`SetCheckpointAtPlayer`**: three separate gaps, all fixed:
  1. The `self+0xa9`-byte-to-`+0xd0` copy needed a `u8 *p = self+0xa9;
     u8 v = *p; p += 0x27; *p = v;` shape (read, then bump the *same*
     pointer, then store) to reproduce the ROM's "derive `+0xd0` by
     adding `0x27` to the register that still holds `+0xa9`" addressing
     - a plain `*(p+0x27) = *p;` computes the destination address
       before the read instead.
  2. In the `mode == 3` branch specifically, this compiler noticed
     `self + 0xa9` is `(self + 0xcc) - 0x23` (the two field addresses
     differ by a compile-time constant) and reused the already-live
     `self+0xcc` pointer via `subs r1, #0x23` instead of the ROM's
     fresh `adds r0, r6, #0; adds r0, #0xa9`. An `asm volatile("" :
     "+r"(self));` barrier right after the `+0xcc` write stops this
     value-numbering reuse without costing an extra instruction, and
     `register u8 *p asm("r0") = self + 0xa9;` then lands the
     recomputed pointer in the same register the ROM uses.
  3. The `self+0xd4`/`self+0xd8` two-word position store (`x`/`y` from
     `gPlayer`) had the exact same "two independent
     raw-offset stores recompute the address twice" gotcha as
     `SetCrateGemPos`/`SetCheckpoint` above - fixed the same way, with a
     local `s32 *dst = (s32 *)(self + 0xd4); dst[0] = x; dst[1] = y;`
     so the second store reuses `[r0, #4]` off the first store's base
     register instead of an extra `adds r0, #4`. This one was only
     caught by the full clean `make compare` (it shifted the whole
     ROM's checksum by 4 bytes) - the isolated per-function compile
     looked byte-identical operand-by-operand and only the *size* was
     off, exactly the kind of gap `docs/workflow.md` step 3 warns an
     isolated compile can't catch.
  The `0x04000040` control-word-reload-per-call and `gEntityFlags`-
  in-`r4` fixes from the original parked note both held up unchanged.

## Update: `PackSaveData` matched

Now byte-exact matched (full clean `make compare`:
`crashbandicootxs.gba: La suma coincide`), leaving only `SetupRoomBlend`
parked in this chunk (since also matched - see the update below).
`asm/code_3_2_17_236ec.s` (which held only this one function's raw
bytes) is deleted; its `ldscript.txt` line is removed.

- **Missing return value.** The real fix wasn't a register-allocation
  trick at all: `PackSaveData` actually returns the `self+0x14c`
  snapshot pointer it just wrote through, as `void *` - the earlier
  parked attempt treated it as `void`. This is externally visible
  already: `pause_menu.c`/`save_menu_input.c` both declare
  `extern void *PackSaveData(void *arg0);` and use the result as a
  pointer (`self->field_10 = PackSaveData(...)`, and as the `src`
  argument to `SummarizeProgress`), so those call sites were already correct
  and needed no changes. Because that pointer is already sitting in r0
  at the end of the function, the epilogue's LR-restore register
  naturally lands on r1 instead of r0 - reproducing the ROM's
  `pop {r1}; bx r1` (instead of the `pop {r0}; bx r0` the earlier
  `void`-returning attempt got) with no extra hint needed, exactly the
  "epilogue register choice follows the function's real shape" pattern
  documented for `JetpackIsPauseLocked`/`PolarIsPauseLocked` in
  [issue-49-0x08029e4c-actor.md](./issue-49-0x08029e4c-actor.md).
- **`self` pinned to r3 for the whole function**, matching the ROM
  (natural codegen instead folds `self` into each field access as an
  immediate-offset addressing mode).
- **`self->0x14d` via register+register indexing**: reproduced with
  two opaque `asm volatile` accesses (`ldrb`/`strb` with explicit
  `r5`/`r3` operands) instead of a precomputed byte pointer, matching
  the ROM's literal-`0x14D`-in-r5-plus-r3 addressing exactly.
- **The first field's `~0x7f` mask**: this compiler narrows `x &
  ~0x7f` down to an 8-bit `mov r1, #0x80` AND once it can prove `x` is
  byte-ranged (from the preceding `ldrb`), but the ROM keeps the full
  32-bit `~0x7f` value, built as `mov r1, #0x80` followed by a
  negate (`rsbs`/`neg`, same encoding) - the same "freshly loaded
  value and its transformed result computed via a separate idiom"
  shape already seen elsewhere in this file. Reproduced with a small
  opaque `asm volatile("mov %0, #0x80\n\tneg %0, %0")` for just that
  one mask (suffix-less `rsb`/`rsbs` text is rejected by
  `arm-none-eabi-as` in this project's Thumb16 mode - `neg` assembles
  to the identical bytes and was used instead).
- **Statement ordering matters as much as register pins here**: the
  ROM loads each "other" word field (`self->0x74`/`0x6c`/`0x78`)
  *before* computing the mask/pointer for its paired byte/halfword
  access, in each of the three packs. Writing the C in that same
  left-to-right statement order (word field first, as its own
  statement; mask/byte access second) was enough for this compiler to
  reproduce the ROM's instruction order without any extra pinning
  beyond the registers already pinned for value shape.

## Update: `SetupRoomBlend` matched

Now byte-exact matched (full clean `make compare`:
`crashbandicootxs.gba: La suma coincide`), closing out the last
function in this chunk. `asm/code_3_2_17_240e4.s` (which held only this
one function's raw bytes) is deleted; its `ldscript.txt` line is
removed.

The dense byte-level bitfield packing (~40 AND/OR/shift/mask
instructions rebuilding the `gBlendRegs` `REG_BLDCNT`/
`REG_BLDALPHA` shadow word) stayed exactly as impractical to hand-pin
register-by-register as the original parked note described. What
closed it was the same "one continuous opaque `asm volatile` island"
technique `AllocVramTileBlock` (`src/gfx/sprite_frame.c`)
and `UpdateActorPaletteCycle` (`src/graphics/actor_part53.c`) established: instead
of fighting this compiler's natural register allocation instruction by
instruction, the whole sequence (both the `if`- and `else`-branch
bodies) is transcribed directly from the ROM disassembly as one literal
instruction stream, using the ROM's own exact register layout
(`self->0x18` in r4, the shadow-word base in r6, mask/value pairs
threaded through r0-r3/r7).

That alone wasn't quite enough, though: wrapped in an ordinary
`asm volatile` block inside a normal (non-`NAKED`) C function, every
instruction byte-matched except the prologue/epilogue. The ROM pushes
and pops all four of `r4`-`r7` in one instruction each
(`push {r4, r5, r6, r7, lr}` / `pop {r4, r5, r6, r7}`), but this
compiler's auto-generated prologue only saves a callee-saved register
it can see a live use for - since r7 here only ever appears in the
asm block's clobber list (nothing gives it a live C-level value), the
compiler silently dropped it from both the push and pop list
(confirmed with a minimal isolated-compile repro:
`push {r4, r5, r6, lr}` / `pop {r4, r5, r6}`, r7 missing from both).
This is the same "gcc-2.9 r7-pin bug" already documented project-wide
(`src/menus/power_dialog_draw.c`'s `DrawPowerDialog`,
`src/graphics/graphics_loading_21280.c`'s `SpawnRoomExit`, among
others) - an explicit `register T x asm("r7")` pin doesn't reliably
survive here either. The fix was the same project-wide escape hatch
those functions already use: mark the function `NAKED` and write the
`push`/`pop` by hand as part of the same literal instruction stream,
rather than relying on the compiler's own prologue/epilogue codegen.

One assembler-compatibility detail carried over from the earlier
parked functions in this cluster: the ROM's `rsbs r1, r1, #0` (and its
`else`-branch twin, `rsbs r0, r0, #0`) had to become `neg r1, r1`/
`neg r0, r0` - a suffixed `rsb`/`rsbs` in text is rejected by
`arm-none-eabi-as` in this project's Thumb16 mode ("cannot honor width
suffix"), but `neg` assembles to the identical encoding. Every other
suffixed ROM mnemonic (`movs`/`ands`/`orrs`/`lsls`) translated to its
suffix-less form (`mov`/`and`/`orr`/`lsl`) with no issue. A `.pool`
right after the `if`-branch's trailing `b 3f` forces the
`gBlendRegs`/`gLevelLayers` literals (loaded via the
assembler's own `=symbol` syntax) to group in the same ROM-matching
mid-function gap the ROM's own `.align 2, 0` + two `.4byte` entries
occupy, right before the `else`-branch, instead of at the function's
end.
