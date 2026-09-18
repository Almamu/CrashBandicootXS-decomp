# Issue #54: 0x0802D3A8-0x0802E0A4 (actor)

**Naming note:** these files are numbered `actor_part58`-`62` rather
than `actor_part57`-`61` (which would have matched their creation order
more naturally) because issue #19's parallel PR independently claimed
`actor_part57.c`/`57b.c` first, before this PR merged - resolved as a
rename on merge to avoid an add/add filename collision. The whole
five-file family was renumbered together (not just the one that
literally collided) to keep it visually contiguous.

25-function `decomp-chunk` covering `asm/code_3_2_20_28568_c99c.s`'s
`sub_802D3A8`-`sub_802E058`/`nullsub_27` range. Two distinct object
families live in this chunk:

- The `InitActorPart`/`gUnknown_03000884`-rooted "self" object family
  already documented for `actor_part17.c`/`actor_part18.c`/
  `actor_part19.c`/`actor_part50.c` and others: a "part table" pointer
  at `self+0`, a table-index/"kind" field at `self+0xc`, an anim-frame
  halfword/byte pair at `self+0x10`/`self+0x12`, an accumulator at
  `self+8`, state at `self+0x28`, a frame counter at `self+0x44`, the
  movement-threshold-cached position triple at `self+0x1c`/`self+0x20`/
  `self+0x24`, and a `+0x50`-rooted event/trampoline table fed through
  `sub_803AD80`. Most of `sub_802D490`-`sub_802D6A0` are constructor
  variants and small state-machine steps on this object.
- `docs/rom_map.md`'s documented `gUnknown_030014BC`-rooted "position-
  tracking object with tier-threshold sound cues" (see "A fourth
  vtable table, a third RAM-struct family" onward): an accumulate-then-
  clamp-at-`0xA000` pair on `gUnknown_030014C8`/`030014CC`, driven from
  `gStaticData_0817A7F8` (a `{s32,s32,s32}` table, stride `0xc`, indexed
  by `gUnknown_030014D4`), branching to tier-keyed `PlaySfx`/
  `sub_80019F8` sound cues, and a VRAM gauge-tile bitmap generator/DMA
  setup (`sub_802DE70`/`sub_802E058`) plus a palette-gradient cursor
  (`sub_802D9A8`/`sub_802DA68`). `sub_802DB2C`/`sub_802DCC0` are two of
  `gStaticData_0817A840`'s four vtable slots operating on this object
  (the other two, `sub_802AB08`-family, were already matched in issue
  #50 under a different chunk).

The raw source file `asm/code_3_2_20_28568_c99c.s` (spanning far beyond
this chunk) has been split around each matched/parked/left-raw
function, following this project's "cut at the boundary" convention -
new fragments are named by the lower 5 hex digits of their first
function's address (`..._d3a8.s`, `..._d7b0.s`, `..._dd9c.s`,
`..._e058.s`, and the tail continuation `..._e0a4.s`).

## Matched (18 of 25 functions)

- **`sub_802D490`** (`src/graphics/actor_part58.c`) - resets a
  different, `gUnknown_03001494`-rooted sibling object via
  `sub_80231EC(gUnknown_030012C0, 0)` then `sub_802D204(self, 0)`.
- **`sub_802D4B0`/`sub_802D4EC`** (`src/graphics/actor_part58.c`) - an
  Aku-Aku-mask-style add/remove pair on `gUnknown_030012C0`'s `+0x78`
  counter (floored at 0 / capped at 3), each playing a sound and
  calling `sub_802D204`.
- **`sub_802D528`/`sub_802D5D4`/`sub_802D648`/`sub_802D764`**
  (`src/graphics/actor_part58.c`) - `InitActorPart`-based constructor
  variants, each installing a different `self+0x50` event table
  (`gStaticData_087E5054`/`5074`/`5094`/`50B4`) before a small amount of
  table-specific setup: `sub_802D528` offsets its position args by
  fixed deltas and pushes a 6th argument through `sub_80231EC`;
  `sub_802D5D4` is a plain passthrough clearing the `+0x2c` one-shot
  flag; `sub_802D648` classifies `posY>>8` into a 3-way "kind" selecting
  which anim record seeds `self+0x10`/`0x12`; `sub_802D764` only
  transitions to kind 2 when `sub_802973C()` matches its own `d`
  argument.
- **`sub_802D57C`/`sub_802D590`** (`src/graphics/actor_part58.c`) -
  trivial: a pass-through-second-argument forwarder to `sub_80231EC`,
  and a getter for `gUnknown_030012C0`'s `+0x78` counter.
- **`sub_802D59C`/`sub_802D600`/`sub_802D6A0`**
  (`src/graphics/actor_part58.c`) - small state-machine steps gated on
  `sub_802A6EC`'s trampoline-fire edge and/or `sub_802DD9C`'s AABB
  overlap test, each ending in `sub_802A7B8`'s frame-advance.
  `sub_802D6A0` needed a fresh, separately-pinned zero register
  (`register u8 zero asm("r1") = 0;`) for its `self[0x12] = 0` stores -
  writing a plain `0` literal let this compiler reuse the already-zero
  "kind" local instead of reproducing the ROM's own extra `movs r1,#0`,
  silently dropping 2 bytes per occurrence (4 bytes total, caught only
  by the full-link `make compare`, see "A note on isolated-compile
  confidence" below).
- **`sub_802DB2C`/`sub_802DCC0`** (`src/graphics/actor_part59.c`) - the
  `gUnknown_030014BC` object's accumulate/clamp/tier-cue/transition
  pair (see the chunk header above). Both needed the `dummyStack`/
  `stackPtr`-style local (a real `u8` whose address is taken and pinned
  to `r4`, matching a genuine `sub sp,#4` stack reservation) so the
  `sub_80019F8` calls' implicit stack-passed byte argument came out as
  a `strb` rather than a promoted `str`; both needed their two tier
  branches' `PlaySfx`+`sub_8029E28`/`sub_80019F8`+`sub_8029E28` call
  pairs written out as genuinely separate call sites (an
  `asm volatile("" ::: "memory")` barrier after the first, or just not
  sharing them via an if/else-if with identical tails) since this
  compiler's own cross-jump optimization otherwise merges the call
  instruction itself across both branches even though the ROM's real
  build only shares the *tail after* the call, not the call; and both
  needed the `gStaticData_0817A7F8 + 4/+8 + idx*0xc` threshold address
  built as `table = gStaticData_0817A7F8; offset = idx*0xc; tablePlusN =
  table + N; *(tablePlusN + offset)` (offset computed *before* the `+N`
  local, forcing the ROM's own `ldr r3,=table; ...; adds r3,#N; adds
  r1,r1,r3` order) rather than folding the whole expression into one
  pointer arithmetic statement, which this compiler instead compiles
  into a single pre-added literal-pool constant.
- **`sub_802DFBC`/`sub_802DFC8`/`sub_802DFDC`**
  (`src/graphics/actor_part60.c`) - the `gUnknown_030014BC` object's
  state-flag setter, destructor (`mem_free`), and constructor
  (`mem_alloc` + part-table wiring + position-tracking reset +
  `sub_802DE70`). `sub_802DFDC` needed the incoming-argument-register
  copies (`gUnknown_030014D4 = arg0`, then `&gUnknown_030014BC` into
  `r5`) and the `mem_alloc` argument setup (`size` into `r0`, `flags`
  into `r1`) each written as a single combined `asm volatile` block per
  group to force this compiler's argument-register-copy order to match
  the ROM's (it otherwise processes `r0`-`r3` in a fixed, source-order-
  independent sequence) - and the `gStaticData_0817A850`/`0817A880`/
  `0xf` triple written as three named locals assigned before any of the
  three stores, so all three loads happen before any store (this
  project's established "compute both loads before either use"
  technique, extended to three).
- **`nullsub_27`** (`src/graphics/actor_part61.c`) - a genuine no-op
  stub.

## Parked (1 of 25 functions, `NON_MATCHING`)

- **`sub_802D3A8`** (`asm/code_3_2_20_28568_c99c_d3a8.s`, C in
  `src/graphics/actor_part62.c`) - eases `self`'s cached position
  (`self+0x1c`/`0x20`/`0x24`) toward a per-state target: state 0 eases
  toward a per-frame-counter table-scatter offset, state 1 snaps
  directly to a different table offset, any other state eases toward
  the raw caller-supplied position. "Easing" is a round-toward-zero
  divide (plain C division reproduces the ROM's own rsb/lsr/add/asr
  idiom exactly, same as `sub_80070EC` in the frozen `docs/matching.md`).
  Logic, register choices (`self`→r5, `posX`→r6, `posY`→ip via
  `register s32 posY asm("ip")`, `posZ`→r7) and every individual
  instruction body are confirmed correct in isolation; parked on two
  compounding gaps:
  1. The prologue's argument-register-copy order: the ROM does `mov ip,
     r2` *before* `adds r7, r3, #0`, but this compiler always emits the
     `r7` copy first regardless of C statement order, pin declaration
     order, or param declaration order. A single combined
     `asm volatile` block with the four copies spelled out in the ROM's
     literal order *does* fix this (see `sub_802DFDC`'s use of the same
     technique above, which worked) -
  2. - but doing so re-pins `posZ` to `r7` for the whole function body,
     and any explicit `r7` pin gets silently clobbered by unrelated
     scratch constant loads later in the function (confirmed 3+
     independent ways: with `posZ` pinned alone, with all four values
     pinned, and with the combined-asm-block prologue fix layered on
     top - every variant produced the exact same `mov r7, #0` /
     `ldr r7, .Lxx` scratch-reuse bug overwriting the live position
     value before its real use). This is the same categorical `r7`-pin
     limitation already documented for `sub_8007DBC` in
     `actor_part2.c` and `matching_decomp_register_pinning` memory
     point 10 ("r7 cannot be pinned in this toolchain, ever") - not
     something more C-level rephrasing is likely to fix, so parked here
     rather than continuing to chase it.

## Left untouched (6 of 25 functions)

- **`sub_802D7B0`** (`asm/code_3_2_20_28568_c99c_d7b0.s`) - one of two
  confirmed slots (index 3) of the type-0 `category_vtable`
  (`gStaticData_081756C4[0]`, `include/actor_anim.h`); also runs a full
  3-axis AABB overlap test against the player (`gUnknown_03000884`)
  before calling `sub_800014C`, one of `UpdateGameFrame`'s own direct
  top-level callees (see `docs/rom_map.md`'s "Two new type-0 vtable
  slots confirmed" section). Not confidently understood well enough for
  a byte-exact reconstruction attempt without real risk of a wrong
  guess about the 12-byte AABB-record layouts involved (two different
  shapes are read: a `gStaticData_0817AA98`-rooted static record and
  `self+0x38`'s own vector, combined via a `sub_800014C`-copied self-
  overlap check whose purpose isn't fully clear) - left raw.
- **`sub_802D9A8`/`sub_802DA68`** (`asm/code_3_2_20_28568_c99c_d7b0.s`)
  - a palette-gradient cursor pair for the `gUnknown_030014BC` object:
  `sub_802D9A8` computes a scale factor via `sub_803ADB4` from
  `gUnknown_030014CC` against two threshold constants (`0x4FFF`/
  `0xBDFF`) and DMAs (or directly writes, in the third branch) a 16-
  color gradient derived from `gStaticData_0817AA6C` into BG palette RAM
  (`0x050001E0`); `sub_802DA68` seeds/arms companion hardware sound
  registers (`0x0400000C`/`0x04000020`/`0x04000028`/`0x0400002C`) keyed
  off `gUnknown_030014C1`/`030014C0`. Semantics are legible but the
  exact palette-index-packing bit math isn't confidently understood -
  left raw.
- **`sub_802D7B0`'s the same shared AABB-overlap-test tail, factored
  out as its own function, `sub_802DD9C`** (`asm/code_3_2_20_28568_c99c_dd9c.s`)
  - a self-vs-player 3-axis overlap test used by `sub_802D6A0`
  (matched, above) among others. Left raw for the same reason as
  `sub_802D7B0` - the two differently-shaped 12-byte record layouts
  involved aren't confidently pinned down yet.
- **`sub_802DE70`** (`asm/code_3_2_20_28568_c99c_dd9c.s`) - a
  ~160-instruction VRAM gauge-tile bitmap generator: sets the DISPCNT
  OBJ-window-enable bit, then runs two nested 16x16 loops (heavy
  `r8`/`sb`/`sl` register pressure) building a triangular-fill dot
  pattern into a stack buffer, DMA3-transferring it to two VRAM tile
  slots (`0x0600D000`/`0x0600D800`), clears a third tile
  (`0x0600BFC0`-`0x0600BFFC`), then arms the object and fires a
  `sub_803AD80` trampoline call. Semantics are legible (this is the
  same shape `sub_802E058` below implements half of, parameterized) but
  the full function's register pressure and DMA-timing interplay wasn't
  attempted for a byte-exact reconstruction here - left raw.
- **`sub_802E058`** (`asm/code_3_2_20_28568_c99c_e058.s`) - a
  parameterized twin of `sub_802DE70`'s triangular-fill loop, taking
  the destination buffer (`arg0`) and seed value (`arg1`) as real
  parameters instead of the fixed stack buffer/globals - left raw for
  the same reason as `sub_802DE70`.

## A note on isolated-compile confidence

`sub_802D6A0`'s missing `movs r1,#0` (see above) is another confirmed
instance of this project's recurring isolated-compile pitfall: the
function's own isolated compile looked byte-identical to the ROM at
every instruction *position*, but reusing an already-zero local instead
of loading a fresh zero silently drops instructions the ROM's real
build keeps - caught only once this whole batch was cut into its real
`src/graphics/actor_part58.c`/`59.c`/`60.c`/`61.c` files and fragment
`.s` files, and the full-link `make compare`'s SHA1 mismatch was
diagnosed via the file-offset → ROM-address → `crashbandicootxs.map`
symbol-address method `docs/workflow.md` describes (a data-segment
pointer literal 8 bytes off pointed at the total code-size shrink
before it; bisecting each matched function's linked address against its
expected ROM address in the map file isolated the two exact functions,
`sub_802D6A0` and `sub_802DB2C`/`sub_802DCC0`'s threshold-address
folding, that had actually drifted).

See [docs/status/actor.md](../status/actor.md) for the running
matched/parked/left-raw list this entry feeds into.
