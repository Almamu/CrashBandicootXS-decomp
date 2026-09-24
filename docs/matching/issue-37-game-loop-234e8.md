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
bitfield accessor pair at `self+0x14c`/`0x14d`, the `sub_8022468`
mode-trampoline family, a DMA3/VRAM refresh pass gating on
`self+0x0 <= 0x1000` ("near start of level"), a `REG_BLDCNT`/
`REG_BLDALPHA` shadow-word rebuild (see `src/graphics/aabb_util.c`'s
`sub_8001624`, which commits that same shadow to hardware), and the
level-end teardown/VRAM-flush tail. Sandwiched in the middle of all
that: two large functions (`sub_802375C`, ~300 instructions;
`sub_8023A1C`, ~650 instructions, its own internal jump-table state
machine with cross-branch `goto`-style jumps) that read like the real
level-start dispatcher and its post-processing continuation - clearly
important, but not confidently understood branch-by-branch within this
pass's scope, so left completely untouched.

## Matched (24 functions, full clean `make compare` passing)

`src/system/game_loop10.c` (`sub_80234E8`-`sub_80236EC`, 15 fns):
`sub_80234E8`/`sub_80234F4` (camera-position field setters),
`sub_8023500` (two-word position setter), `sub_8023510`/`sub_802352C`
(busy-flag setters gated on `sub_8023290`/`sub_80232B8`),
`sub_8023548`/`sub_802356C` (checkpoint snapshot restore/stash pair,
the latter also flushing two spans of the `gUnknown_030012B4` bitmap
via the `CpuSet` wrapper), `sub_80235E4` (progress-accumulate-or-reset
dispatcher), `sub_802364C`/`sub_8023658`/`sub_802369C` (the
`sub_8022468` mode-trampoline family, one of them also playing a fixed
SFX), `sub_8023674` (allocates a `0x44c`-byte block and hands it to
`sub_8037154`), `nullsub_24` (empty stub), `sub_80236AC` (bitfield
unpacker, refreshing its own snapshot first), `sub_80236EC` (its
packer inverse - see the update below, added after this doc's original
pass).

`src/system/game_loop11.c`: `sub_8023738` (lazy-allocates and returns
`gUnknown_03000828`) - its own file since the still-raw
`sub_802375C`/`sub_8023A1C` pair sits on both sides of it in ROM order.

`src/system/game_loop8.c`: `sub_802400C` (the DMA3/VRAM refresh pass).

`src/system/game_loop9.c` (`sub_8024198`-`sub_802423C`, 5 fns):
`sub_8024198`/`sub_80241A4`/`sub_80241B0` (a boolean flag
clear/set/get trio on `gUnknown_03000830`), `sub_80241BC` (level-end
teardown: DMA-copies the level's first palette word into `PLTT`,
clears it, then re-runs `sub_802400C`'s refresh pass and four
`fade_screen_mode2.c` state resets), `sub_802423C` (the shared
vram-upload-cursor/OAM-shadow flush tail both `sub_802400C` and
`sub_80241BC` end with).

### Gotchas worth recording

- **`sub_8023500`/`sub_802356C`'s two-word field copies**: writing
  `dst->field0 = x; dst->field1 = y;` as two independent raw-offset
  stores makes this compiler compute two full addresses from scratch.
  The ROM computes the base pointer once and uses immediate-offset
  stores off it - reproduced by declaring a local `s32 *dst` pointer
  and indexing `dst[0]`/`dst[1]` instead of re-deriving the address
  each time (see also the pre-load-both-then-store-both ordering below,
  needed when the *source* is also a two-word read).
- **`sub_802356C`'s repeated `0x04000040` `sub_803A94C` control word**:
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
  reason `sub_802356C` isn't in the first commit of this branch's
  history - the isolated per-function compile looked identical to the
  ROM but the full-ROM `make compare` (as `docs/workflow.md` step 6
  warns) caught the real byte offset.
- **`sub_8023548`/`sub_802356C`'s "read-then-relocate-pointer" byte
  copies** (`self->0xa9 = self->0xd0`, `self->0xd0 = self->0xa9`):
  writing the assignment directly lets the compiler compute the
  destination address before reading the source in one case, or fuse a
  same-object offset into a `+4`-style relative access that doesn't
  match the ROM's independent address derivation in another. Fixed
  with an explicit `u8 tmp = *src; *dst = tmp;` two-step, forcing the
  read to happen (and the value to be captured) before the destination
  address is computed.
- **`sub_8023674`'s `nullsub_7` call**: the ROM keeps the freshly
  allocated block's pointer in r0 *across* the `bl nullsub_7` call and
  only moves it to a callee-saved register afterward - only possible
  because `nullsub_7` (now matched separately, in
  `src/audio/counter_selector.c`) was compiled in the same translation
  unit as this function originally, so the compiler could prove it
  doesn't touch r0. Split across files, an ordinary C call must
  conservatively assume r0-r3 are clobbered and moves the pointer to
  r4 *before* the call - one instruction too early. Reproduced by
  spelling that one call as inline asm (`asm volatile("bl nullsub_7" :
  "+r"(tmp) :: "r1", "r2", "r3", "lr", "cc")`) that tells the compiler
  the pointer register survives, which is true here and lets the
  delayed move happen exactly where the ROM has it - the *next* call
  (`sub_80361B0`, a real, unrelated function) still forces the normal
  conservative move beforehand, so this isn't a general "keep values in
  r0 forever" trick, just an accurate description of this one no-op
  call's real effect.
- **`sub_80236AC`'s byte/halfword extracts**: the ROM loads each raw
  byte/halfword into one register and computes the shifted result into
  a *different* one (`ldrb r1,[r5]` then `lsls r0,r1,#0x19`), rather
  than shifting in place - the same "freshly-loaded value and its
  transformed result in different registers" pattern documented
  elsewhere in `docs/matching.md`. Fixed with explicit `register`
  pins for the raw value (r1) and the shifted result (r0), reusing r5
  (the snapshot pointer) for the halfword load too, matching the ROM's
  register reuse exactly.
- **`sub_802400C`'s `sub_803AD7C` reload pattern**: computing
  `gUnknown_030012D8` and its `->table` field into plainly-named
  locals both times let the compiler pick whichever register was
  convenient rather than reloading through r0 the way the ROM does at
  both call sites (needed since the global's value can change as a
  side effect of the first `sub_803AD7C` call). Fixed by pinning the
  reloaded `struct actor *` to r0 at each of the two call sites.

## Parked (`NON_MATCHING`) - 1 function

- **`sub_80240E4`** (`src/system/game_loop8.c`, real bytes in
  `asm/code_3_2_17_240e4.s`) - rebuilds the `gUnknown_03001280`
  `REG_BLDCNT`/`REG_BLDALPHA` shadow word from a level object's
  raster-mode fields. Every field/mask/shift/branch is confirmed
  correct, but this compiler's natural register allocation for the
  dense byte-level bitfield packing picks a different register for
  nearly every intermediate than the ROM throughout (which keeps
  `self->0x18` in r4, the shadow-word base in r6, and threads
  mask/value pairs through r0-r3/r7 in a specific reused order) -
  plausible but impractical to hand-pin every one of the ~40
  instructions involved.

## Left raw - 2 functions

- **`sub_802375C`/`sub_8023A1C`** (`asm/code_3_2_17_2375c.s`, ROM
  `0x0802375C`-`0x08024007`) - a ~300-instruction level-start
  dispatcher (allocates and initializes several HUD/counter widget
  objects via `sub_8026EDC`+`sub_800B69C`/`sub_803AD80`, dispatches on
  a 3-way record-type switch) feeding into a ~650-instruction
  continuation (`sub_8023A1C`) built around a 6-case jump table with
  cross-branch jumps into a shared tail (`_08023BB8`/`_08023BBE`
  setting a state flag and jumping into the middle of a *different*
  branch's cleanup code, `_08023E72`). The overall shape (spawn a HUD
  widget set, run a per-frame update loop gated on `sub_80241B0`,
  react to a completion signal from `sub_8004D74`) is legible, but
  several callees (the `gStaticData_0816C8xx` tables' exact record
  shape, `sub_8027018`'s 6-argument signature, `sub_80266BC`,
  `sub_800B3F0`, `sub_8026F54`) aren't characterized precisely enough
  yet to commit to a byte-exact reconstruction of this size with
  confidence - left untouched rather than force a low-confidence
  match. A good next target once the HUD-effect-queue family
  (`sub_8027018`/`sub_8026F54`, currently left raw per
  `tools/report_units.py`'s `hud` entries) is better understood.

See [docs/status/game_loop.md](../status/game_loop.md) for the running
matched/parked/raw lists this updates.

## Update: `sub_8022BF0`/`sub_8022CA0` matched

Both of this entry's two parked functions are now byte-exact matched
(full clean `make compare`: `crashbandicootxs.gba: La suma coincide`),
closing out the last of the original 25-function chunk. `asm/code_3_2_17_22bf0.s`
(which held only these two functions' raw bytes) is deleted; its
`ldscript.txt` line is removed.

- **`sub_8022BF0`**: the earlier attempt cached all four of
  `self+0x70`/`0x6c`/`0x74`/`0xbc` behind pointer locals, which spilled
  into `r8`/`r9`/`sl`. The fix: `self+0x70`/`0x6c`/`0x74` all fit the
  Thumb `ldr`/`str` immediate range (0-124) and the ROM addresses them
  directly off `self` with no cached pointer at all - only
  `0xb4`/`0xb0`/`0xb8`/`0xd4`/`0xe0`/`0xbc` (all past that range)
  actually need an address computed into a local. Dropping the three
  unnecessary pointer locals brought the whole function down to the
  ROM's exact `r4`-`r7` register set, no spill.
- **`sub_8022CA0`**: three separate gaps, all fixed:
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
     `gUnknown_030012D8`) had the exact same "two independent
     raw-offset stores recompute the address twice" gotcha as
     `sub_8023500`/`sub_802356C` above - fixed the same way, with a
     local `s32 *dst = (s32 *)(self + 0xd4); dst[0] = x; dst[1] = y;`
     so the second store reuses `[r0, #4]` off the first store's base
     register instead of an extra `adds r0, #4`. This one was only
     caught by the full clean `make compare` (it shifted the whole
     ROM's checksum by 4 bytes) - the isolated per-function compile
     looked byte-identical operand-by-operand and only the *size* was
     off, exactly the kind of gap `docs/workflow.md` step 3 warns an
     isolated compile can't catch.
  The `0x04000040` control-word-reload-per-call and `gUnknown_030012B4`-
  in-`r4` fixes from the original parked note both held up unchanged.

## Update: `sub_80236EC` matched

Now byte-exact matched (full clean `make compare`:
`crashbandicootxs.gba: La suma coincide`), leaving only `sub_80240E4`
parked in this chunk. `asm/code_3_2_17_236ec.s` (which held only this
one function's raw bytes) is deleted; its `ldscript.txt` line is
removed.

- **Missing return value.** The real fix wasn't a register-allocation
  trick at all: `sub_80236EC` actually returns the `self+0x14c`
  snapshot pointer it just wrote through, as `void *` - the earlier
  parked attempt treated it as `void`. This is externally visible
  already: `settings_menu15.c`/`settings_menu8b.c` both declare
  `extern void *sub_80236EC(void *arg0);` and use the result as a
  pointer (`self->field_10 = sub_80236EC(...)`, and as the `src`
  argument to `sub_80048E0`), so those call sites were already correct
  and needed no changes. Because that pointer is already sitting in r0
  at the end of the function, the epilogue's LR-restore register
  naturally lands on r1 instead of r0 - reproducing the ROM's
  `pop {r1}; bx r1` (instead of the `pop {r0}; bx r0` the earlier
  `void`-returning attempt got) with no extra hint needed, exactly the
  "epilogue register choice follows the function's real shape" pattern
  documented for `sub_802A674`/`sub_802A688` in
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
