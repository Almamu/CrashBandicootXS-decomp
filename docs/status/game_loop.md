# Status: game_loop

The top-level per-frame game loop - `MainLoop`, `UpdateGameFrame`, and
the state/counter machinery they drive. Filed under `src/system/` on
disk, tracked as its own `game_loop` category since `docs/rom_map.md`
and the `decomp-chunk` issue generator both treat it as a distinct
system from "core" system startup/init code.

## Matched

- `src/system/game_loop.c` (GitHub issue #34): `sub_8022BF0`
  (level-start/checkpoint-restore progress-total updater) and
  `sub_8022CA0` (its cached-state/snapshot helper) - see
  [docs/matching/issue-37-game-loop-234e8.md](../matching/issue-37-game-loop-234e8.md)
  for the register-allocation fixes that closed these two out.
- `src/system/main_loop.c` (new file, GitHub issue #45 - categorized
  `hud` by the chunk generator, but `MainLoop` itself is squarely
  `game_loop`): `MainLoop` - the game's actual top-level loop (called
  once from `AgbMain`, sets up the central per-level state object and
  the on-screen counter widget, then runs `UpdateGameFrame` forever) -
  and `sub_8026F38`, a two-level per-counter-widget-mode table lookup
- `src/system/game_loop2.c`: `sub_8022FEC`, `sub_802306C`, `sub_8023104`,
  `sub_8023110`, `sub_8023118`, `sub_8023120`, `sub_8023128`,
  `sub_8023130`, `sub_8023138`, `sub_8023140`, `sub_802314C`,
  `sub_8023158`, `sub_8023168`, `sub_8023184`, `sub_8023190`,
  `sub_802319C`, `sub_80231A8`, `sub_80231B4`, `sub_80231BC`,
  `sub_80231C4` (GitHub issue #34, `UpdateGameFrame`-`MainLoop` cluster -
  a `self+0x80`/`0x84`/`0x88`/`0xac`/`0xc0`/`+2`-flags accessor family
  plus the two frame-counter/limit tick functions)
- `src/system/game_loop3.c` (GitHub issue #40): `sub_8024E68`,
  `sub_8024E90`, `sub_8024EB4` (a viewport/parallax-scroll-layer object)
  and `sub_8024F04`/`sub_8024F0C`/`sub_8024F10`/`sub_8024F14`/
  `sub_8024F18`/`sub_8024F1C`/`sub_8024F20` (its field accessors)
- `src/system/game_loop4.c` (GitHub issue #40): `sub_8025444`,
  `nullsub_4`, `sub_8025460` (matched `NAKED`, closing the whole issue
  #40 terrain-tile-cache cluster - see below)
- `src/system/game_loop5.c` (GitHub issue #40): `sub_80254C0`,
  `sub_80254F8`, `sub_8025554`, `sub_8025588`, `sub_80255A8`,
  `sub_80255C4` - the terrain tile-record decode cache's constructor,
  a raw-cell-lookup variant, a floor-div-by-32 bitmap set/clear pair,
  and a `CpuSet`-based palette-bank zero-fill wrapper pair
- `src/system/game_loop10.c` (GitHub issue #37 - numbered `10` rather
  than `6` since issue #12's parallel PR independently claimed
  `game_loop6.c`/`game_loop7.c` first): `sub_80234E8`,
  `sub_80234F4`, `sub_8023500`, `sub_8023510`, `sub_802352C`,
  `sub_8023548`, `sub_802356C`, `sub_80235E4`, `sub_802364C`,
  `sub_8023658`, `sub_8023674`, `sub_802369C`, `nullsub_24`,
  `sub_80236AC`, `sub_80236EC` - camera-position setters,
  checkpoint/level-transition snapshot helpers, the `sub_8022468`
  mode-trampoline family, and a packed-bitfield unpacker/repacker pair
- `src/system/game_loop11.c` (GitHub issue #37): `sub_8023738` - lazily
  allocates and returns `gUnknown_03000828`
- `src/system/game_loop8.c` (GitHub issue #37): `sub_802400C` - the
  DMA3/VRAM refresh pass gated on `self+0x0 <= 0x1000` - and
  `sub_80240E4` - the `REG_BLDCNT`/`REG_BLDALPHA` shadow-word rebuild,
  matched `NAKED` via a literal ROM-instruction transcription (a
  gcc-2.9 r7-pin bug drops r7 from the compiler's own prologue/epilogue
  push/pop otherwise). See
  [docs/matching/issue-37-game-loop-234e8.md](../matching/issue-37-game-loop-234e8.md)
  for the details.
- `src/system/game_loop39.c` (GitHub issue #37, follow-up pass):
  `sub_802375C` - the level-start dispatcher that allocates the
  per-level HUD widget set, the player actor, and the text-box
  singleton, then dispatches on a widget-kind field to construct one of
  three counter/ring-buffer widgets before handing off to the still-raw
  `sub_8023A1C`. See
  [docs/matching/issue-37-game-loop-2375c.md](../matching/issue-37-game-loop-2375c.md)
  for the register-pinning/evaluation-order gotchas that closed this
  out.
- `src/system/game_loop9.c` (GitHub issue #37): `sub_8024198`,
  `sub_80241A4`, `sub_80241B0`, `sub_80241BC`, `sub_802423C` - a
  boolean flag clear/set/get trio, the level-end teardown, and the
  shared vram-upload-cursor/OAM-shadow flush tail
- `src/system/game_loop12.c` (GitHub issue #41): `sub_8025894`
  (group/item list counter with a 19-entry jump table - previously
  `NON_MATCHING`, now matched as real C by materializing the
  `item->type == 0x1a` four-load lookup chain as one opaque
  `asm volatile` block using only `r0`/`r1`, matching the ROM's own
  two-scratch-register reuse pattern, plus splitting the loop-bound
  `i` init into two statements so the count loads directly into `i`'s
  own register instead of a scratch register first - see
  [issue-41-game-loop-25894.md](../matching/issue-41-game-loop-25894.md)'s
  "closed" update), `sub_8025944`,
  `sub_8025968`, `sub_802599C` - the first two of three overlapping
  bit-grid accessors at `self+8`/`self+0x208`/`self+0x308`
- `src/system/game_loop13.c` (GitHub issue #41): `sub_80259D4`
  (sets a bit in both the `self+0x208` and `self+0x308` bit-grids at
  once - previously NAKED, now matched as real C via an
  inline-asm-materialized self-stash/n-copy pair plus a second local
  keeping the ROM's own untouched `n`-copy register alive for later
  reuse - see
  [naked-sub_80259d4-matched.md](../matching/naked-sub_80259d4-matched.md)),
  `sub_8025A0C`
  (third bit-grid setter), `sub_8025A3C` (Q8-to-int store),
  `sub_8025A44` (conditional `sub_8026ED0` forward), `sub_8025A5C`
  (zero two Q8 words)
- `src/system/game_loop14.c` (GitHub issue #41): `sub_8025D28`
  (table-indexed function-pointer dispatch via the interworking
  trampoline convention), `sub_8025D4C` (store two Q8 words),
  `sub_8025D54` (conditional `sub_8026ED0` forward, dup of
  `sub_8025A44`), `sub_8025D6C` (zero two Q8 words, dup of
  `sub_8025A5C`)
- `src/system/game_loop15.c` (GitHub issue #41): `sub_8025D74`
  (BG-scroll-layer hardware-register/bitfield initializer - previously
  NAKED, now matched as real C via opaque inline-asm-materialized mask
  folds plus one function-owned literal pool for its three pointer-sized
  constants - see
  [naked-sub_8025d74-matched.md](../matching/naked-sub_8025d74-matched.md)),
  `sub_8025DE8`,
  `sub_8025E2C` (streamed-tile-range growers firing a `self->0x30`-
  table trampoline per step), `sub_8025E70`, `sub_8025E84` (their
  plain clamp-only counterparts)
- `src/system/game_loop16.c` (GitHub issue #41): `sub_8025F24` -
  truncates the Q8 position to a tile-scroll halfword pair and writes
  it through the `self+0x58` hardware-register pointer
- `src/system/game_loop17.c` (GitHub issue #38): `sub_802425C`,
  `nullsub_25`, `sub_8024278` - a bit-tested `sub_8026ED0` teardown
  wrapper, an empty stub, and the medal-table per-level tally
- `src/system/game_loop18.c` (GitHub issue #38): `sub_80243E0`,
  `sub_8024404`, `sub_8024428`, `sub_8024434`, `sub_8024440`,
  `sub_802444C`, `sub_8024458`, `sub_8024464`, `sub_8024498`,
  `sub_80244F0`, `sub_8024524`, `sub_8024540`, `sub_802455C` -
  medal-table entry/item-list field accessors, the sound-cue resolver,
  and the `sub_8024344` constant wrappers (`sub_8024344` itself is left
  raw, see below)
- `src/system/game_loop19.c` (GitHub issue #38): `sub_8024784` -
  trivial `gUnknown_03001314` setter
- `src/system/game_loop20.c` (GitHub issue #38): `sub_80247EC`,
  `sub_8024804` - the `sub_802425C`-shaped teardown wrapper and a
  trivial constructor
- `src/system/game_loop22.c` (GitHub issue #13 - numbered `22` rather
  than `17` since issue #38's PR above independently claimed
  `game_loop17.c`-`20.c` first): `sub_800FEB0` - resets
  `self`'s collision-response state/timer/neighbor-list-pointer block
  on reset
- `src/system/game_loop23.c` (GitHub issue #13, third pass for
  `sub_8010674`): `sub_8010674` (AABB-overlap test between `self`'s
  own table-driven half-width/half-height box and a caller-supplied
  box, prepended ahead of the rest since it's immediately
  ROM-adjacent), `sub_80106DC` (viewport collision-box refresh),
  `sub_8010708`/`sub_801070C` (neighbor-list "get prev"/"get next"),
  `sub_8010710`/`sub_8010714` ("set prev"/"set next"), `sub_8010718`
  (UNUSED trivial constant). See
  [docs/matching/issue-13-fc70-second-continuation.md](../matching/issue-13-fc70-second-continuation.md)
  for `sub_8010674`'s register-pinning/toolchain-bug notes.
- `src/system/game_loop24.c` (GitHub issue #13): `sub_8010804` (a
  state-3-countdown-expiry sweep over `gUnknown_0300130C`),
  `sub_801085C` (viewport trampoline-pair/cue-1 firing)
- `src/system/game_loop25.c` (GitHub issue #13): `sub_8010908` -
  trivial `gStaticData_0816BBAE[idx]` lookup
- `src/system/game_loop26.c` (GitHub issue #13): `sub_8010A00` -
  `self+0x48` bits 6-7 sub-state extractor
- `src/system/game_loop29.c` (GitHub issue #13, second pass): `sub_801089C`
  - cue-3 SFX plus a `gUnknown_030012B4` bit-grid consume-if-clear and a
  `sub_8025A64` part-object spawn. See
  [docs/matching/issue-13-fc70-continuation.md](../matching/issue-13-fc70-continuation.md).
- `src/system/game_loop30.c` (GitHub issue #13, second pass):
  `sub_8010914`/`sub_801095C` (the "get prev"/"get next"
  neighbor-list-walk-and-filter helpers - previously NAKED, now matched
  as real C via source-order block placement matching the ROM's own
  layout plus an inline-asm-materialized mask check - see
  [naked-sub_8010914-matched.md](../matching/naked-sub_8010914-matched.md))
  and `sub_80109A4` - a distance-gated `sub_0800D18C` dispatcher
  clearing `self+0xc` bit 3. See
  [docs/matching/issue-13-fc70-continuation.md](../matching/issue-13-fc70-continuation.md).
- `src/system/game_loop31.c` (GitHub issue #13, second pass): `sub_801071C`/
  `sub_801075C` (a part-object table-set/tail-call-`sub_8008484` helper
  pair) and `sub_8010784`/`sub_80107C4` (two fixed single-octant
  Bresenham-line-style step algorithms). See
  [docs/matching/issue-13-fc70-continuation.md](../matching/issue-13-fc70-continuation.md).
- `src/system/game_loop33.c` (GitHub issue #13, third pass): `sub_800FDC8`
  - the full 4-octant Bresenham-line-style line-stepper `sub_8010784`/
  `sub_80107C4` (game_loop31.c) are fixed single-octant variants of -
  previously NAKED, now matched as real C via the same source-order
  block-placement technique as `sub_8010914`/`sub_801095C` (a `goto`
  to a physically-earlier shared-return label) plus an opaque-asm
  materialization of the one octant case whose own return must stay
  physically separate from that shared tail (plain-C placement alone
  wasn't enough here - this compiler's cross-jump pass still unified
  it with the shared copy purely by instruction content) and per-case
  `diff`/`err` register pins (`r6`/`r0`, the ROM's own fixed roles) -
  see [naked-sub_800fdc8-matched.md](../matching/naked-sub_800fdc8-matched.md).
- `src/system/game_loop36.c` (GitHub issue #13, fourth pass, new file -
  replaces the trimmed `asm/code_3_2_17_e560_ff0c.s`, now deleted):
  `sub_800FF0C` - the `sub_800FF0C` entity-constructor trampoline
  family's own target function (two whole files, `graphics_loading_21bfc.c`/
  `graphics_loading_21668.c`, exist purely to call it with a fixed
  `type` constant). Allocates a 0x64-byte object, sets `self+0x18` to
  `&gStaticData_087E4074` (a `+0x18` outlier of the usual `+0xC`
  table-pointer convention), then dispatches on `type` (0-0x12,
  externally confirmed by every trampoline caller) through two nested
  jump tables (15 and 19 cases) to tag `self+0x2d` and initialize
  per-type fields - including a confirmed call to `sub_800F5B8`
  (game_loop49.c, issue #12) for `type == 5`. `NAKED` transcription
  (byte-correct, not real decompiled C): three extended registers
  (`r8`/`sb`/`sl`) live across the whole function, the same
  gcc-2.9-resistant shape already established for `sub_0800D18C`/
  `sub_800E08C` (game_loop47.c/game_loop48.c). See
  [docs/matching/issue-13-0x0800ff0c-graphics.md](../matching/issue-13-0x0800ff0c-graphics.md)
  for the full type-code-to-behavior table.
- `src/system/game_loop35.c` (GitHub issue #13, third pass, new file -
  it sits between `sub_800FF0C` (now `game_loop36.c`) and `sub_80104E4`,
  so it can't join either neighbor's file): `sub_8010480` - a
  `self+0x4d`-gated reset of `self+0x30`/`self+0x38` via the
  `self+0x20`-pointer-to-manager/`self+0x2d`-tag/0x1c-stride
  hitbox-record convention `sub_800D040` (game_loop6.c) also uses,
  then a tail call to `sub_8007A84`. See
  [docs/matching/issue-13-fc70-second-continuation.md](../matching/issue-13-fc70-second-continuation.md).
- `src/system/game_loop18.c` (GitHub issue #38, follow-up pass): `sub_8024344`
  (medal item-list per-flag nonzero scan) - prepended ahead of
  `sub_80243E0`, contiguous with `game_loop17.c` in ROM. See
  [docs/matching/issue-38-sound-channel-family.md](../matching/issue-38-sound-channel-family.md)
  for the `ip`/r12 pin plus the pointer-arithmetic-canonicalization
  gotcha that closed this out.
- `src/system/game_loop37.c` (GitHub issue #38, follow-up pass):
  `sub_8024640` (per-item sound-channel driver loop), `sub_80246D8`
  (its "find next active item" index scanner), and - as of a second
  follow-up pass - `sub_8024708` (VRAM-bank tile-asset streamer) too.
  `sub_8024590` in the same file is NAKED-parked, see below. See
  [docs/matching/issue-38-sound-channel-family.md](../matching/issue-38-sound-channel-family.md)
  and its second-pass addendum.
- `src/system/game_loop38.c` (GitHub issue #38, follow-up pass):
  `sub_8024790` - the tail half of `sub_8024640`'s per-item body, reused
  standalone. See
  [docs/matching/issue-38-sound-channel-family.md](../matching/issue-38-sound-channel-family.md).
- `src/system/game_loop27.c` (GitHub issue #14, recategorized
  graphics->game_loop - a direct continuation of the same physics/
  collision subsystem file family): `sub_8010A0C`-`sub_8010B68` (24
  functions) plus the unlabeled `sub_8010AF8` (the original
  disassembly never gave it its own symbol - it falls out of
  `sub_8010AEC`'s trailing alignment padding) - a run of bit-field get/
  set/clear accessors and plain field accessors on the same
  "collision box" record `sub_8010A00`/`sub_800FEB0` already operate
  on. See
  [docs/matching/issue-14-0x08010a0c-graphics.md](../matching/issue-14-0x08010a0c-graphics.md).
- **`sub_8022EA8`/`sub_8022F2C`** (`src/system/game_loop2.c`, GitHub
  issue #34) - record 47's periodic-trigger setter/decrementer; closed
  with a targeted register-pinning recipe after the ROM's cross-call
  `r4`/`r7` register map was reproduced by pinning only the two values
  that need it (see the functions' own doc comments for the full
  recipe: an early `base` snapshot forcing `r0`, a single `off`
  register pin for the `0x234` field-offset constant, freshly-named
  locals for the second chase to stop register "stickiness", and an
  `addr`/`countdown`/`newCountdown` pin set plus true-branch-first
  digit-cascade rewrites for `sub_8022F2C`'s front half and `else`
  branch). Real bytes formerly in `asm/code_3_2_17_22ea8.s` (now
  removed, folded into `src/system/game_loop2.o`).
- **`sub_8026628`** (`src/system/game_loop43.c`, new file - dedicated
  deep investigation) - independently flagged "still unexamined" from
  two other closed call sites this session (`sub_8009BE0`'s physics/
  collision step-probe and `sub_800AAEC`'s input-action-check gate) and
  sketched in `docs/rom_map.md` as an umbrella dispatcher unifying
  `sub_8026AE8`/`sub_8026A18` under one API. A small (148 B) 4-arm
  `switch` on `mode`, matched on the first isolated-compile attempt
  with no register pins needed - see
  [docs/matching/issue-9-10-41-0x08026628-game-loop.md](../matching/issue-9-10-41-0x08026628-game-loop.md).
- **`sub_8026A18`**/**`sub_8026AE8`** (`src/system/game_loop46.c`, new
  file - closing pass on `sub_8026628`'s own axis resolvers, semantics
  already fully derived by that investigation) - the Y-axis (floor/
  ceiling) and X-axis (wall) tile-scan resolvers, 208/216 B. Same
  gcc-2.9 register-allocation-permutation gap already forced NAKED on
  `sub_8025130`/`sub_8025228`/`sub_8025460`/`sub_8024F24` next door
  (`self`/`pos`/`outValue`/`hit` packed into `sl`/`r7`/`r8`/`sb`
  simultaneously) - an isolated-compile attempt at plain C never
  reproduced the ROM's own register assignment even with register
  pins (pinning made it worse, the same symptom `sub_8024F24`'s own
  comment already documented). Closed both as hand-transcribed NAKED
  functions instead - see
  [docs/matching/issue-9-10-41-0x08026628-game-loop.md](../matching/issue-9-10-41-0x08026628-game-loop.md).
- **`sub_8026BC0`** (`src/system/game_loop44.c`, new file - dedicated
  deep investigation) - independently flagged "still raw" by two
  already-documented callers (`sub_800A884`'s camera-probe tail and a
  jump-table dispatch context in `sub_8007634`'s own write-up). A
  56-byte wrapper around the already-matched terrain-tile-cache lookup
  `sub_8025460` (`game_loop4.c`, GitHub issue #40): converts `(x, y)`
  to that cache's lookup units via a plain `>>3` clamped to `>= 0` on
  each axis independently, then calls `sub_8025460` and returns only
  the flags byte it also returns directly (the `hi` out-param is
  discarded, unread by either known caller). Matched on the first
  isolated-compile attempt with no register pins needed - see
  [docs/matching/issue-9-10-0x0800a884-graphics.md](../matching/issue-9-10-0x0800a884-graphics.md).
- **`sub_8026BF8`/`sub_8026C3C`/`sub_8026C80`/`sub_8026C8C`**
  (`src/system/game_loop45.c`, new file - GitHub issue #9/#10, matching
  pass on functions already fully understood from
  `docs/matching/issue-9-0x0800a178-graphics.md`) - the single-point
  terrain-height ("floor") probes `sub_800A178`/`sub_800A420`
  (`src/graphics/actor_part110.c`) call. Both `s32 fn(void *player,
  struct probe_pos *pos, s32 *outValue)`: `sub_8026BF8` reads a signed
  height byte via the raw terrain streamer `sub_80250BC`; `sub_8026C3C`
  gets it via the CheckTerrainFlag-style `sub_8025228`. `sub_8026C3C`
  matched on the first isolated-compile attempt; `sub_8026BF8` needed a
  narrow register-pinned inline-asm materialization of `ldrsb` (this
  agbcc build never emits Thumb `LDRSB` from any C-level signed-byte
  array read - confirmed categorically with a minimal standalone test -
  always lowering to `ldrb`+shift instead). Bonus pass also closed the
  two tiny functions immediately following, `sub_8026C80`/`sub_8026C8C`
  - both UNUSED (no caller anywhere in the ROM), matched anyway per this
  project's usual practice. `asm/code_3_2_17_26bf8.s` trimmed to begin
  at `sub_8026C90`.
- **`sub_800E620`/`sub_800ED08`** (`src/system/game_loop48.c`, new file
  - GitHub issue #12 Phase 2, lower-address half) - two of
  `sub_0800D18C`'s/`sub_800E08C`'s per-edge jump-table dispatch
  targets, matched as real C: `sub_800E620` (dispatch id `0xe`)
  switches `self` into hitbox tag `0x14`, rebuilds its hitbox record,
  and re-derives a low-nibble sub-animation value via the shared
  `+0x20`-table/`+0x2d`-tag convention's own `sub_8006DF8` tile-asset-
  cache lookup - needing several register-pinned/inline-asm-anchored
  blocks for this compiler's usual operand-materialization-order and
  register-choice gaps in this shape (a field-store's immediate loaded
  before vs. after the field address, a byte-mask computed via runtime
  negation instead of a folded 8-bit AND immediate, and which of two
  operands' registers an OR's result lands in); `sub_800ED08`
  (dispatch id `0xf`) is a small `self+0x48 & 7` state switch
  forwarding to `sub_800EAFC`/`sub_800EEF0` or spawning a bonus object.
  See
  [docs/matching/issue-12-physics-collision.md](../matching/issue-12-physics-collision.md)'s
  Phase 2 writeup.

See [docs/workflow.md](../workflow.md) for the per-function loop, and
[docs/matching.md](../matching.md) for gotchas encountered along the way.

## Parked - NAKED transcription (byte-correct, not decompiled)

These are byte-exact (confirmed by a full clean `make compare`), but
as `NAKED` functions whose body is the ROM's own disassembly
transcribed instruction-for-instruction rather than real decompiled C,
they don't count as "matched" for this project's tracking - the goal
is readable C, and an asm blob wrapped in a C function signature
doesn't advance that even when byte-correct. See
[docs/workflow.md](../workflow.md)'s NAKED-transcription escape hatch
(`sub_8001CB8`/`sub_8001DB4` in `src/system/link_cable.c`) for the
established convention, and each entry's linked write-up for why
plain C didn't converge.

- **`sub_800D040`** (`src/system/game_loop6.c`, GitHub issue #12) -
  builds `self`'s and the player's AABB from the shared
  `+0x20`-table-pointer/`+0x2d`-tag hitbox-record convention
  (`sub_8007B00`/`sub_8007B98` in `actor_part.c`), dispatches to
  `sub_800EEF0`/`sub_800E7A8` on overlap. See
  [docs/matching/issue-12-physics-collision.md](../matching/issue-12-physics-collision.md).
- **`sub_0800D18C`/`sub_800E08C`** (`src/system/game_loop47.c`, new
  file - GitHub issue #12 Phase 1) - the physics/collision subsystem's
  two largest, most tangled dispatchers, closed by NAKED transcription
  rather than real C: `sub_0800D18C` (~3840 B, not the ~1960 B this
  issue's original read-only pass estimated) is the subsystem's
  collision-response commit - three nested jump tables (a 7-case hitbox
  selector, the 6-case per-edge handler dispatch
  `docs/rom_map.md`/this issue's write-up already described, and a
  9-case post-processing dispatch), a 5-slot "recently touched" ring
  buffer inside `gUnknown_030012D8`, and ~30 distinct callees.
  `sub_800E08C` (1032 B) is a further 6-case jump-table dispatcher
  `sub_0800D18C` itself calls into, sharing the exact same per-edge
  handler family. Both verified byte-exact via a full clean
  `make compare`. `asm/code_3_2_17_d18c.s` is now gone entirely - see
  [docs/matching/issue-12-physics-collision.md](../matching/issue-12-physics-collision.md)'s
  Phase 1 appendix for the confirmed dispatch maps (the basis for this
  issue's Phase 2 parallel split of the remaining 18 leaf functions).
- **`sub_800E560`/`sub_800E6B0`/`sub_800E7A8`/`sub_800E888`/
  `sub_800EAFC`/`sub_800EDBC`** (`src/system/game_loop48.c`, new file -
  GitHub issue #12 Phase 2, lower-address half) - NAKED transcriptions
  of the direct dispatch targets both `sub_0800D18C`'s and
  `sub_800E08C`'s per-edge jump tables call (`sub_800E560`,
  `sub_800E6B0`, `sub_800E7A8`) plus their own transitive callees
  (`sub_800E888`, called from `sub_800E7A8`; `sub_800EAFC`/
  `sub_800EDBC`, called from `sub_800E888`) - jump-table-heavy
  (`sub_800E888`'s own 23-case table is the largest in this subsystem
  after `sub_0800D18C`'s three) and/or built on this subsystem's
  confirmed `r8`/`sb`/`sl`-triple-accumulator-resistant shape
  (`sub_800EDBC`). See
  [docs/matching/issue-12-physics-collision.md](../matching/issue-12-physics-collision.md)'s
  Phase 2 writeup.
- **`sub_800EEF0` through `sub_800F990`** (12 functions:
  `sub_800EEF0`, `sub_800F06C`, `sub_800F1B8`, `sub_800F258`,
  `sub_800F2BC`, `sub_800F368`, `sub_800F4F4`, `sub_800F5B8`,
  `sub_800F6B8`, `sub_800F798`, `sub_800F8E0`, `sub_800F990` -
  `src/system/game_loop49.c`, new file - GitHub issue #12 Phase 2,
  higher-address half, sibling pass) - the direct/transitive callees of
  `sub_0800D18C`'s and `sub_800E08C`'s per-edge jump table reachable
  from `sub_800EEF0` up through the end of this whole cluster
  (0x0800EEF0-0x0800FC70). All twelve closed by NAKED transcription for
  the same gcc-2.9-resistant register-shape reasons as Phase 1's two
  dispatchers - each re-triggers either the `+0x20`/`+0x2d`-hitbox-record
  AABB-build idiom or plain high-register (`r8`/`sb`/`sl`) cross-block
  reuse under -O2. Verified byte-exact via a full clean `make compare`.
  `asm/code_3_2_17_e560.s` is now gone entirely - both this half and
  the lower-address half above (`src/system/game_loop48.c`) are fully
  consumed. See
  [docs/matching/issue-12-physics-collision.md](../matching/issue-12-physics-collision.md)'s
  Phase 2 appendix for the confirmed per-function roles.
- **`sub_800E494`/`sub_800E4E4`** (`src/system/game_loop7.c`, GitHub
  issue #12) - bidirectional linked-list walkers (`sub_801070C`/
  `sub_8010708`) clearing/setting each neighbor's `+0x58` flag. See
  [docs/matching/issue-12-physics-collision.md](../matching/issue-12-physics-collision.md).
- **`sub_800CEAC`/`sub_800CF70`** (`src/system/game_loop42.c`, new
  file - dedicated deep investigation) - the two functions formerly
  tracked as unexamined raw bytes between `sub_800CD00` (issue #9/#10)
  and `sub_800D040` (issue #12); recategorized `graphics` -> `game_loop`
  since both are called only from `sub_0800D18C`. `sub_800CF70` walks
  `self`'s `sub_801070C`/`sub_8010708` neighbor list, sets a caller
  out-param when either exists, and - for the "prev" neighbor, gated by
  the subsystem's own `self+0x4d&0x7f==1` exclusion - builds its AABB
  via the shared `+0x20`-table convention and tests it against a
  caller-supplied box, confirming and sharpening `docs/rom_map.md`'s
  existing partial note on this function. `sub_800CEAC` builds a hybrid
  AABB (the player's hitbox quad positioned at `self`'s location,
  optionally widened when player state byte `+0x90` is set) and tests
  it the same way. Both NAKED: the same single-inlined-AABB-build shape
  `sub_800D040`/`sub_800CD00` already document as gcc-2.9-resistant. See
  [docs/matching/issue-9-10-0x0800ceac-graphics.md](../matching/issue-9-10-0x0800ceac-graphics.md).
- **`sub_800FC70`** (`src/system/game_loop32.c`, GitHub issue #13,
  second pass) - a per-frame position-wrap advance keeping `sb`/`r8`
  live as two extra callee-saved accumulators throughout, the same gap
  as `sub_800D040` above. See
  [docs/matching/issue-13-fc70-continuation.md](../matching/issue-13-fc70-continuation.md).
- **`sub_800B8DC`**/**`sub_800BD48`** (`src/graphics/actor_part112.c`,
  new file - GitHub issue #9/#10, foundational investigation of the
  large still-raw `0x0800B8DC`-`0x0800D040` cluster). Two independent
  entity-vtable slots of the *same* object type (`gStaticData_087E3EE4`)
  sitting adjacent in ROM but never calling each other.
  `sub_800B8DC` (1132 B) is an 18-state dispatcher on `self+0x74` (the
  same "stateful widget" field shape `sub_800C6A8`'s `menu_ui` dialogs
  and `sub_800CD00` also use) - most states delegate to a handful of
  further `self+0x68`-dispatching siblings, five have real inline logic
  (a distance-band velocity-target gate, a position-anchor cache, a
  conditional directional-target trigger, a landing/jump-impulse
  handler, and a proximity-ambient-sound-plus-popup-text handler).
  `sub_800BD48` (608 B) is a second, shallower 22-case dispatcher on its
  own third argument - 17 of the 22 states are no-ops, the other two
  distinct paths are an ambient-sound-spawn-plus-reflag tail and a
  spawn-and-launch-a-child-object handler. Both NAKED: the same
  `self`/`owner`-multi-field register-allocation gap as every other
  entry in this section. See
  [docs/matching/issue-9-10-0x0800b8dc-graphics.md](../matching/issue-9-10-0x0800b8dc-graphics.md)
  for the full 18-case and 22-case dispatch maps.
- **`sub_800C8AC`/`sub_800C8BC`/`sub_800C8CC`** (`src/graphics/actor_part113.c`,
  new file - GitHub issue #9/#10, Phase 2 of the `0x0800B8DC`-cluster
  investigation above, tackling the three `(self, mode)`-shaped trigger
  primitives that pass's own doc flagged as shared by nearly every
  dispatch state). All three real C, matched clean - much smaller and,
  unlike `sub_800B8DC`/`sub_800BD48`, free of the `self`/`owner`
  register-allocation gap (straight-line, no branches). `sub_800C8AC`/
  `sub_800C8BC` cache `mode` into `self+0x7c`/`self+0x78` and delegate
  to the already-matched `sub_800B704`/`sub_800B838`
  (`actor_part17.c`); `sub_800C8CC` caches into `self+0x68` and fires
  `sub_803AD84` directly, indexing `self+0x84`'s own pointer array by
  `mode` rather than going through the shared `gStaticData_0816B304`
  table the other two use. See
  [docs/matching/issue-9-10-0x0800b8dc-graphics.md](../matching/issue-9-10-0x0800b8dc-graphics.md)'s
  "Phase 2" section for the full writeup.
- **`sub_800C18C`/`sub_800C1E8`/`sub_800C314`/`sub_800C8F8`/
  `sub_800C940`/`sub_800C97C`/`sub_800C9C8`/`sub_800CBD4`**
  (`src/graphics/actor_part114.c`-`actor_part117.c`, new files - GitHub
  issue #9/#10, Phase 3 of the `0x0800B8DC`-cluster investigation, the
  "remaining leaves" the Phase 1 doc's priority list named). `sub_800C18C`/
  `sub_800C1E8` are the X-axis/Y-axis "homing velocity-target setter"
  pair; `sub_800C314` is state 7's `self+0x68`-dispatched callee (a
  mirror-flag toggle plus a wrapping 0-3 counter advance);
  `sub_800C8F8`/`sub_800C940`/`sub_800C97C` are a `gStaticData_0816A820`
  sine-wave-oscillator family. All six matched as NAKED - each hit a
  *different* gcc-2.9/this-agbcc-build code-selection gap (branch-
  polarity/cross-jump-merging differences for the first pair, bit-
  toggle instruction-sequencing for the third, constant-materialization
  and register-copy-operand choices for the oscillator trio) despite
  isolated real-C attempts getting the full branch/dispatch structure
  and even established idioms like the `(s32)(x<<27)<0` mirror-flag
  test right - see the doc's own "Phase 3" section for the full
  per-function breakdown. `sub_800C9C8` (a thin `sub_8025B0C` wrapper,
  state 18's floating-popup spawner) and `sub_800CBD4` (`sub_800BD48`
  states 19-20's child-object allocator, resolving `self+0xc` to the
  fixed `gStaticData_087E3FA4` table) both matched as real C. See
  [docs/matching/issue-9-10-0x0800b8dc-graphics.md](../matching/issue-9-10-0x0800b8dc-graphics.md)'s
  "Phase 3" section for the full writeup.
- **`sub_800C6A8`/`sub_800C860`/`sub_800C87C`/`sub_800C898`**
  (`src/graphics/actor_part122.c`, new file - GitHub issue #9/#10, the
  last four functions of the old `asm/code_3_2_17_c6a8.s`, now fully
  retired). `sub_800C6A8` is the `menu_ui` dialog-widget system's own
  18-state `self+0x74` update, called from all 31 confirmed `menu_ui`
  dispatch-table entries - despite the "menu_ui" framing it turns out to
  run on the exact same `self`/`owner`/`self+0xc`-anchor/`self+0x84`-table
  object shape as the rest of this cluster, and its case bodies manually
  re-inline `sub_800C8AC`/`sub_800C8BC`/`sub_800C8CC`'s own `bl` targets
  rather than calling those three wrapper functions - confirming these
  are literal instances of the same object type, not merely a
  structurally-similar sibling. NAKED: several case groups compile the
  identical inlined `sub_800C8CC(self,0)` sequence at deliberately
  separate, unmerged jump-table addresses, the exact tail-merging trap
  the Phase 3 entry above already documents this agbcc build hitting,
  combined with the same `self`/`owner` register-pressure shape the rest
  of this cluster's dispatchers share. `sub_800C860`/`sub_800C87C`/
  `sub_800C898` (the `self+0x70`-relative X/Y homing-bound accessor
  triple) matched as real C on the first attempt, using a register-pinned
  local plus an empty `asm volatile` barrier to force the ROM's own
  "load owner field, then shift the radius" instruction order. See
  [docs/matching/issue-9-10-0x0800b8dc-graphics.md](../matching/issue-9-10-0x0800b8dc-graphics.md)'s
  "Phase 4" section for the full writeup.
- **`sub_800CBF4`/`nullsub_15`/`nullsub_3`/`sub_800CCCC`/`sub_800CCE0`**
  (`src/graphics/actor_part123.c`, new file - GitHub issue #9/#10, the
  final piece of the `0x0800B8DC`-cluster investigation, closing out
  the entire 43-function cluster). `sub_800CBF4` (NAKED) inlines the
  "flag active + bitmap-set" idiom (`actor_part27c.c`'s `sub_8018884`)
  three times over, each independently gated (a `sub_803AD7C` hit-probe
  reporting no hit, a flags-bit-3 test, and a `+0x38` byte test).
  `nullsub_15`/`nullsub_3` are genuine empty stubs, matched as real C.
  `sub_800CCCC`/`sub_800CCE0` (both real C) are two more constructors in
  the `sub_801886C`/`sub_8018858`/`sub_800CBD4` family, both re-pointing
  `self+0xc` at `gStaticData_087E400C`. `sub_800CCE0` sits right at the
  physics/collision subsystem's own boundary
  ([docs/matching/issue-12-physics-collision.md](../matching/issue-12-physics-collision.md))
  but is confirmed to still be a plain entity constructor in this
  cluster, immediately followed with no gap by the already-matched
  `sub_800CD00` (`actor_part109.c`). See
  [docs/matching/issue-9-10-0x0800b8dc-graphics.md](../matching/issue-9-10-0x0800b8dc-graphics.md)'s
  final section for the full writeup.
- **`sub_800BFA8`** (`src/graphics/actor_part121.c`, new file - GitHub
  issue #9/#10) - the last raw function in the cluster's own
  `asm/code_3_2_17_bfa8.s` chunk, called only from `sub_800B8DC` state
  15. A small `self+0x68`-keyed 2-way dispatcher gated by a
  `sub_803AE4C` "close enough" check against `gUnknown_0300082C` (here
  read as a plain word, not the table-base-pointer role `sub_800C40C`
  uses it in) plus `self->0x48`/`self->0x4c`; on pass, triggers
  `sub_800C8CC(self,2)`/`sub_800C8CC(self,7)` for `self->0x68==0`/`4`.
  On failure, re-dispatches through `owner` (`self->0x70`):
  `owner->0x38` set triggers `sub_800C8CC(self,0)`/`sub_800C8CC(self,4)`
  for `self->0x68==2`/`7`; `owner->0x38` clear instead gates a
  `sub_800C9C8(0xc,6,0,d,0x400,owner)` call (`d=-0xa` for mode 2,
  `d=8` for mode 7) behind an `owner->0x30`/`owner->0x34` magic-constant
  check, tagging the returned record's `+0xa` byte with `8` on success.
  Small enough to avoid this cluster's usual `self`/`owner`
  register-pressure trap - matched as real C, needing `self` pinned to
  `asm("r4")` (gcc's unforced allocator otherwise duplicates `self`
  into a spare `r5` just to re-read `self->0x68` a second time) and the
  `gUnknown_0300082C` read hoisted into its own statement ahead of
  `self->0x48`'s (two independent loads gcc's scheduler otherwise
  reorders vs. the ROM). This fully consumes `asm/code_3_2_17_bfa8.s` -
  retired from `ldscript.txt` entirely. See
  [docs/matching/issue-9-10-0x0800b8dc-graphics.md](../matching/issue-9-10-0x0800b8dc-graphics.md)'s
  "`sub_800BFA8`" entry.
- **`sub_8025A64`** (`src/system/game_loop29.c`, new file - not
  contiguous with any other matched run once its three siblings below
  stayed parked - GitHub issue #41) - a part-object spawn helper; hits
  the confirmed `register T x asm("r7")`-never-saved toolchain bug plus
  an unfixable `& -0x10` mask-fold. See
  [docs/matching/issue-41-game-loop-25894.md](../matching/issue-41-game-loop-25894.md).
- **`sub_8025B0C`/`sub_8025BAC`/`sub_8025CA4`** (`src/system/game_loop14.c`,
  prepended ahead of the already-matched `sub_8025D28` run - GitHub
  issue #41) - three more part-object spawn helpers; `sub_8025BAC`
  alone repeats `sub_8025A64`'s unfixable mask-fold three times over.
  See [docs/matching/issue-41-game-loop-25894.md](../matching/issue-41-game-loop-25894.md).
- **`sub_8025E98`/`sub_8025F3C`** (`src/system/game_loop16.c`, GitHub
  issue #41) - the screen-edge tile-coordinate computer/streaming
  driver, and the circular-buffer decoded-tile streaming loop; both
  keep `r8` live across most of their bodies. See
  [docs/matching/issue-41-game-loop-25894.md](../matching/issue-41-game-loop-25894.md).
- **`sub_8024F24`** (`src/system/game_loop3.c`, GitHub issue #40) - the
  16-slot terrain tile-record decode/LRU cache's lookup dispatcher (16
  fixed `recordId == self->id[N]` checks plus an LRU-evict-and-decode
  miss path). Semantics/control-flow/total size were already fully
  confirmed as real C; the only gap was this compiler's register
  allocator always picking the opposite of the ROM's `self`/`recordId`
  <-> `r7`/`r3` assignment, and explicit register pins on either
  variable making it worse (falls back to `sp`-relative addressing
  instead of using the pinned register as a base pointer at all) - the
  same symptom independently confirmed for `sub_801E688`/
  `LoadGraphicsPackage`/`LoadBg2Background`. See
  [docs/matching/issue-40-terrain-tile-cache.md](../matching/issue-40-terrain-tile-cache.md).
- **`sub_80250BC`/`sub_8025130`/`sub_8025228`** (`src/system/game_loop3.c`,
  GitHub issue #40, follow-up pass) - `sub_8024F24`'s three `(x, y)`
  tile-lookup consumers (a terrain-property-table pointer lookup, its
  `mode`-selected/`flagsOut`-writing sibling, and a `mode`-dispatched
  single-flag-byte variant). Same register-allocation-permutation gap as
  `sub_8024F24` above; closed the same way, including reproducing the
  ROM's own mid-function `.pool` literal-pool splits exactly. See
  [docs/matching/issue-40-terrain-tile-cache.md](../matching/issue-40-terrain-tile-cache.md).
- **`sub_8025334`** (`src/system/game_loop3.c`, GitHub issue #40) - the
  16-slot terrain tile-record decode/LRU cache's RLE/delta
  token-stream decoder. Decode-loop mechanics and control flow were
  already fully confirmed as real C; the gap was that the ROM keeps a
  "bytes-written" byte-offset write pointer alive in `r7` across the
  whole function, using it *directly* as the store target only for the
  delta-run mode's first/last writes while every other write (in all
  three modes) recomputes a fresh `dest + written*2` pointer from
  `r8`/`ip` instead, even though `r7` holds the identical value in
  lockstep - a redundant shadow-register shape no index-based C
  reconstruction reproduces. Closed as NAKED, the same way as its
  siblings above. `asm/code_3_2_17_24f24.s` is now gone entirely - this
  was the last function still living there. See
  [docs/matching/issue-40-terrain-tile-cache.md](../matching/issue-40-terrain-tile-cache.md).
- **`sub_8025460`** (`src/system/game_loop4.c`, GitHub issue #40) - the
  last of `sub_8024F24`'s `(x, y)`-tile-lookup consumers: returns the
  raw decoded halfword directly (no bounds check, no terrain-table
  lookup) while also writing the cell's top nibble out through
  `hiOut`. Same register-allocation-permutation gap as
  `sub_8025130`/`sub_8025228` above; closed the same way, hand-
  transcribed from the ROM disassembly (formerly
  `asm/code_3_2_17_25460.s`, now retired). No mid-function `.pool`
  split was needed - the function has no literal-pool references at
  all. This was the last unclosed member of the issue #40
  terrain-tile-cache cluster - the whole issue is now closed. See
  [docs/matching/issue-40-terrain-tile-cache.md](../matching/issue-40-terrain-tile-cache.md).
- **`sub_8022D50`** (`src/system/game_loop40.c`, GitHub issue #34) - the
  level-start/reset routine (`self+0x8c`/`0x90`-`0xa0` clears, the
  `self+0x1b8`/`0x1bc` actor-slot teardown, the `gUnknown_030012EC`
  array walk firing `sub_803AD7C` trampolines and setting bits in the
  `gUnknown_030012B4+0x108` collision bitmap); every piece matches
  byte-for-byte in isolation, but the loop's own `0xffff` sentinel has
  to live in r7 for the whole array walk and hits the confirmed
  never-adds-an-inline-asm-clobbered-r7-to-the-function's-own-push/pop-
  list toolchain bug once every other quirk in the function is also
  anchored. See
  [docs/matching/issue-34-game-loop-8022d50-80255d4.md](../matching/issue-34-game-loop-8022d50-80255d4.md).
- **`sub_8024590`** (`src/system/game_loop37.c`, GitHub issue #38, second
  follow-up pass) - starts/re-selects a sound cue and plays its secondary
  sfx immediately or after a busy-wait; a second pass on this function's
  previously-NON_MATCHING C reconstruction closed two of its three
  documented gaps for real (a redundant register-copy step, fixed with a
  forced-same-register-move `asm volatile` idiom; a mismatched initial
  item-pointer load, fixed by splitting the transient first-use load from
  the persistent one) but hits the same confirmed
  never-adds-an-inline-asm-clobbered-(or even genuinely written-and-read)-
  r7-to-the-function's-own-push/pop-list toolchain bug as `sub_8022D50`
  above for its busy-poll loop tail, which needs `push {r4, r5, r6, r7,
  lr}`. See [docs/matching/issue-38-sound-channel-family.md](../matching/issue-38-sound-channel-family.md)'s
  second-pass addendum.
- **`sub_80255D4`** (`src/system/game_loop41.c`, GitHub issue #34/#40/
  #41 - the second half of the same follow-up pass as `sub_8022D50`
  above) - `self` is `*gUnknown_030012B4`: a `self+0`-cache-gated DMA3
  zero-fill/`CpuSet` refresh of the `self+8`/`0x208`/`0x108`/`0x308`
  collision-bitmap family, then a `list` group/item walk firing
  `sub_8025D28` trampolines (the same shape `sub_8025894`, matched
  above, documents for a sibling list), then a
  `gUnknown_0300130C`
  actor-list redirect-chain linker/position-sync pass keyed off a
  count-prefixed `redirectInfo` array (every field, offset, branch and
  call argument confirmed - see the linked write-up for the full
  trace). A real C reconstruction got the entire first half
  byte-for-byte once `self`/the group-loop counter were pinned to their
  ROM registers (`r6`/`r7`), but the second half's persistent
  `redirectInfo`-array base lives in `sb`/`r9` in the ROM only as a
  *source* value - at every individual 3-operand-Thumb-add use site
  (`sb` being a high register Thumb restricts there) the ROM re-issues
  a fresh `mov rX, sb` into whichever low register is free at that
  exact point, never the same one twice, where a plain C pointer local
  gets allocated one single register for its whole lifetime instead.
  See
  [docs/matching/issue-34-game-loop-8022d50-80255d4.md](../matching/issue-34-game-loop-8022d50-80255d4.md).
- **`sub_8010B6C`** (`src/system/game_loop28.c`, GitHub issue #14,
  follow-up pass) - the chunk's last and largest function, a
  collision-candidate scan/resolve helper `sub_80106DC`
  (`game_loop23.c`) already calls once a frame. Every field offset/
  branch/call argument was already understood and cross-referenced
  against the mirror-image `sub_8010D54` and its caller when this was
  first parked; the ROM builds nearly every record-field address as a
  running pointer incremented by `0x24` per loop iteration, with up to
  twelve of them (`r8`/`sb`/`sl` included) live across a `0x68`-byte
  stack frame - the same gap already parked for `sub_800A734`/
  `sub_800A528` in docs/matching/issue-9-0x08007634-actor.md, at a
  much larger scale (three times the live-cursor count, on a stack
  frame twice the size) - beyond what C-level register pins can
  realistically express, so closed as a byte-exact NAKED transcription
  instead. `asm/code_3_2_17_e560_10b6c.s` is now gone entirely - the
  function is folded into `src/system/game_loop28.o`. See
  [docs/matching/issue-14-0x08010a0c-graphics.md](../matching/issue-14-0x08010a0c-graphics.md).
- **`sub_80104E4`** (`src/system/game_loop51.c`, new file, GitHub
  issue #13) - a ~195-instruction per-frame state-machine dispatcher:
  throttles/re-triggers `sub_800F8E0`/`sub_800F990`/`sub_800F4F4` off
  `self+0x4e`'s settle-state byte, always calls `sub_800FC70`, then -
  gated on `self+0x4d`'s bit 7 and `self+0x38` - re-derives
  `self+0x30`'s index via the same `self+0x20`/`self+0x2d`-tag/
  0x1c-stride hitbox-record clamp `sub_8010480` (`game_loop35.c`)
  uses and settles state 6/3, or otherwise re-triggers `sub_800F798`;
  finally hands off to the `sub_803AD7C` table-trampoline convention
  `sub_8007048`/`sub_80070D4` (`graphics.c`) establish. A plain-C
  attempt (the same register-pin-per-nested-scope technique that
  matched `sub_8010480`'s near-identical hitbox-record clamp) matched
  the first ~10 instructions but diverged once a *second* field
  address needed the same "computed once, copied to a callee-saved
  register, reused later" shape - this compiler's liveness tracking
  for register-`asm`-pinned locals produced an extra dead register
  copy the ROM never makes. Beyond that, field *addresses* thread
  through r0/r1/r6/r2/r5/r8/ip across many `bl` calls with an
  inconsistent reuse pattern (sometimes recomputed fresh a few
  instructions after an equivalent address was already live) - the
  same "which anonymous scratch register" gap as `sub_800D040`/
  `sub_8010B6C` above, at a finer grain spread across the whole
  function rather than one isolated block. Replaces
  `asm/code_3_2_17_e560_104e4.o` in `ldscript.txt`, sitting between
  `src/system/game_loop35.o` and `game_loop23.o`. Filed as
  `game_loop51.c`, not `game_loop50.c`, after a merge conflict with
  the concurrently-matched `sub_8010D54` below, which took the
  `game_loop50.c` name first. See
  [docs/matching/issue-13-fc70-continuation.md](../matching/issue-13-fc70-continuation.md)'s
  "Update: `sub_80104E4` matched" section.
- **`sub_8010D54`** (`src/system/game_loop50.c`, new file - Phase 1 of
  the next still-unexamined chunk past issue #14's own range) - the
  physics/collision subsystem's **apply/commit step**, the call
  `sub_0800D18C` (`game_loop47.c`) makes at the very end of its own
  per-edge dispatch. Appends one 0x24-byte "collision candidate" record
  to a per-entity queue at `self->candidates[self->count]` (`self` is
  the caller's own `entity+0x108` - the player's
  `gUnknown_030012D8+0x108` at this specific call site - the same
  record shape `sub_8010B6C`/`game_loop28.c` already reads back, per
  its own doc comment calling `sub_8010D54` its "mirror image"). A
  plain-C reconstruction reproduces the ROM's exact instruction *shape*
  (the same 8-way common-subexpression grouping for the repeated
  `self->count` index computation, sharing a computation between
  adjacent field writes in exactly the same places the ROM does) but
  gcc 2.9 -O2 picks a different scratch register for the "copy of
  `self` used to read `self->count`" step almost every time - not one
  isolated register letter to pin, so closed via NAKED transcription
  instead (no branches or literal pool in this function, so no label
  renumbering was needed). Matched, confirmed by a full clean `make
  compare` ("La suma coincide"). `asm/code_3_2_17_e560_10d54.s` trimmed
  to begin at `sub_8010E14` (24 functions still raw, `0x08010E14`-
  `0x080119A8`) - see
  [docs/matching/issue-14-0x08010d54-physics-apply.md](../matching/issue-14-0x08010d54-physics-apply.md)
  for the full semantic map and Phase 2 planning notes on those 24.

## Still raw, category-mapped (GitHub issue #12/#34/#40)

- **24 functions, `0x08010E14`-`0x080119A8`** (`asm/code_3_2_17_e560_10d54.s`,
  trimmed) - the remainder of the chunk `sub_8010D54` (above) was the
  entry point of; still category-mapped `graphics` pending its own
  examination, though the first two functions are confirmed direct
  siblings of `sub_8010D54`'s own collision-queue record - see
  [docs/matching/issue-14-0x08010d54-physics-apply.md](../matching/issue-14-0x08010d54-physics-apply.md)'s
  Phase 2 planning section for the full function/size list.
- **`UpdateGameFrame`** (`asm/code_3_2_17_225a0.s`, ROM `0x080225A0`) -
  the main per-frame game-loop driver, a ~730-instruction jump-table
  state machine. Not understood branch-by-branch with the precision a
  byte-exact reconstruction needs yet - see `docs/matching.md`.
- **`sub_8023A1C`** (`asm/code_3_2_17_23a1c.s`, ROM
  `0x08023A1C`-`0x08024007`, GitHub issue #37) - a ~650-instruction
  jump-table-driven level-lifecycle continuation, called
  unconditionally from `sub_802375C` (now matched, `game_loop39.c` -
  see
  [docs/matching/issue-37-game-loop-2375c.md](../matching/issue-37-game-loop-2375c.md)).
  `sub_8027018`'s call shape is now confirmed against all four of this
  function's own call sites, but the `gStaticData_0816C8xx` tables it
  indexes and a few other callees (`sub_80266BC`, `sub_8023484`) aren't
  characterized precisely enough yet to commit to a byte-exact
  reconstruction of this size - see
  [docs/matching/issue-37-game-loop-2375c.md](../matching/issue-37-game-loop-2375c.md).
