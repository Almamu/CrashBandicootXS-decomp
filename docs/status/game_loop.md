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
  `nullsub_4`
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
  DMA3/VRAM refresh pass gated on `self+0x0 <= 0x1000`
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
- `src/system/game_loop35.c` (GitHub issue #13, third pass, new file -
  it sits between the still-raw `sub_800FF0C` and `sub_80104E4`, so it
  can't join either neighbor's file): `sub_8010480` - a
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
- **`sub_800E494`/`sub_800E4E4`** (`src/system/game_loop7.c`, GitHub
  issue #12) - bidirectional linked-list walkers (`sub_801070C`/
  `sub_8010708`) clearing/setting each neighbor's `+0x58` flag. See
  [docs/matching/issue-12-physics-collision.md](../matching/issue-12-physics-collision.md).
- **`sub_800FC70`** (`src/system/game_loop32.c`, GitHub issue #13,
  second pass) - a per-frame position-wrap advance keeping `sb`/`r8`
  live as two extra callee-saved accumulators throughout, the same gap
  as `sub_800D040` above. See
  [docs/matching/issue-13-fc70-continuation.md](../matching/issue-13-fc70-continuation.md).
- **`sub_800FDC8`** (`src/system/game_loop33.c`, GitHub issue #13,
  second pass) - the full 4-octant Bresenham-line-style line-stepper
  `sub_8010784`/`sub_80107C4` (game_loop31.c) are fixed single-octant
  variants of; every octant case was individually matched as plain C,
  but this compiler's cross-jump pass over-merges one octant's own
  early-return into the shared tail the other three legitimately share
  in the ROM too (4 bytes short). See
  [docs/matching/issue-13-fc70-continuation.md](../matching/issue-13-fc70-continuation.md).
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

## Parked (`NON_MATCHING`, not yet byte-exact)

- **`sub_8010B6C`** (`src/system/game_loop28.c`, GitHub issue #14; real
  bytes in `asm/code_3_2_17_e560_10b6c.s`) - the chunk's last and
  largest function, a collision-candidate scan/resolve helper
  `sub_80106DC` (`game_loop23.c`) already calls once a frame. Every
  field offset/branch/call argument is understood and cross-referenced
  against the mirror-image `sub_8010D54` and its caller; parked on the
  ROM building nearly every record-field address as a running pointer
  incremented by `0x24` per loop iteration, with up to twelve of them
  (`r8`/`sb`/`sl` included) live across a `0x68`-byte stack frame - the
  same gap already parked for `sub_800A734`/`sub_800A528` in
  docs/matching/issue-9-0x08007634-actor.md, at a larger scale. See
  [docs/matching/issue-14-0x08010a0c-graphics.md](../matching/issue-14-0x08010a0c-graphics.md).
- **`sub_8022EA8`/`sub_8022F2C`** (`src/system/game_loop2.c`, GitHub
  issue #34) - record 47's periodic-trigger setter/decrementer; real
  bytes in `asm/code_3_2_17_22ea8.s`. See `docs/matching.md`'s issue
  #34 entry for the exact register-allocation gaps.
- **`sub_8024F24`/`sub_80250BC`/`sub_8025130`/`sub_8025228`/
  `sub_8025334`** (`src/system/game_loop3.c`, GitHub issue #40) - the
  16-slot terrain tile-record decode/LRU cache's lookup dispatcher, its
  four `(x, y)`-lookup consumer variants, and the RLE/delta
  token-stream decoder; real bytes in `asm/code_3_2_17_24f24.s`. See
  [docs/matching/issue-40-terrain-tile-cache.md](../matching/issue-40-terrain-tile-cache.md)
  for the exact register-allocation gaps.
- **`sub_80240E4`** (`src/system/game_loop8.c`, GitHub issue #37) - the
  `REG_BLDCNT`/`REG_BLDALPHA` shadow-word rebuild; real bytes in
  `asm/code_3_2_17_240e4.s`. See
  [docs/matching/issue-37-game-loop-234e8.md](../matching/issue-37-game-loop-234e8.md)
  for both parked functions' exact register-allocation gaps.
## Still raw, category-mapped (GitHub issue #12/#34/#40)

- **`sub_0800D18C`/`sub_800E08C`** (`asm/code_3_2_17_d18c.s`, ROM
  `0x0800D18C`-`0x0800E494`, GitHub issue #12) - the physics/collision
  subsystem's largest, most tangled functions (docs/rom_map.md:
  "Confirmed: a shared physics/collision subsystem, entered from
  multiple different entity types" - `sub_0800D18C` alone is ~1960B, a
  6-case jump-table collision-response commit that maintains a 5-slot
  ring buffer inside `gUnknown_030012D8` and calls 27 other functions
  in this same neighborhood). Not understood branch-by-branch with the
  precision a byte-exact reconstruction needs yet - see
  [docs/matching/issue-12-physics-collision.md](../matching/issue-12-physics-collision.md).
- **`sub_800E560` onward through `sub_800F990`** (`asm/code_3_2_17_e560.s`,
  ROM `0x0800E560`-`0x0800FC70`, GitHub issue #12) - the rest of this
  chunk's 25-function list: the collision-response jump-table handlers
  `sub_0800D18C` itself dispatches to (`sub_800E620`, `sub_800E6B0`,
  `sub_800E7A8`, `sub_800E888`, `sub_800EAFC`, `sub_800ED08`,
  `sub_800EDBC`, `sub_800EEF0`, `sub_800F06C`, `sub_800F1B8`,
  `sub_800F258`, `sub_800F2BC`, `sub_800F368`, `sub_800F4F4`,
  `sub_800F5B8`, `sub_800F6B8`, `sub_800F798`, `sub_800F8E0`,
  `sub_800F990`), each a moderately-sized state-machine function with
  several sibling calls within this same still-raw neighborhood; left
  untouched for this pass - see
  [docs/matching/issue-12-physics-collision.md](../matching/issue-12-physics-collision.md).
- **`sub_800FF0C`** (`asm/code_3_2_17_e560_ff0c.s`, ROM `0x0800FF0C`,
  GitHub issue #13) - a large (~660-instruction) projectile/
  hazard-spawn dispatcher with two big jump tables and packed bitfield
  arguments; still out of scope for a single pass - see
  [docs/matching/issue-13-fc70-second-continuation.md](../matching/issue-13-fc70-second-continuation.md)
  (`sub_8010480`/`sub_8010674`, the two functions that used to share
  this raw span with it, are now matched - see `game_loop35.c`/
  `game_loop23.c` above).
- **`sub_80104E4`** (`asm/code_3_2_17_e560_104e4.s`, ROM `0x080104E4`,
  GitHub issue #13) - a further ~195-instruction state dispatcher
  calling several still-raw siblings; not attempted - see
  [docs/matching/issue-13-fc70-second-continuation.md](../matching/issue-13-fc70-second-continuation.md).
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
