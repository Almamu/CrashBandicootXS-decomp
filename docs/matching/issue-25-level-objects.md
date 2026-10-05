# Issue #25: 0x0801A794-0x0801B85C, graphics - level objects and their platform mover

All 25 functions of the former `asm/code_3_2_17_188d0_1a794.s` are now in C
(the file is deleted). **24 are real C, byte-exact; 1 is parked as a NAKED
transcription** (`ResolvePlatformCollision`) with its near-miss C reconstruction kept
under `#if NON_MATCHING`. Verified with a clean `make compare`
(`crashbandicootxs.gba: OK`). `CreatePlatform` was NAKED too until the
old_agbcc retry (docs/matching/old-agbcc-retry.md): its object is now
built with `tools/agbcc/bin/old_agbcc` (Makefile `OLD_AGBCC_OBJS`), under
which it matches with all of its register pins removed.

The range is split by address into five objects so that
`tools/report_units.py` can track the NAKED function as unmatched (and so
`CreatePlatform` could move to old_agbcc on its own):

| file | functions | state |
|---|---|---|
| `src/graphics/actor_part_1a794.c` | `CreateDingodileShieldCtrl`-`SetDingodileNextState` (6) | matched |
| `src/graphics/actor_part_1a878.c` | `CreatePlatform` | matched (old_agbcc) |
| `src/graphics/actor_part_1ab34.c` | `CheckPlatformContact` | matched |
| `src/graphics/actor_part_1ab98.c` | `ResolvePlatformCollision` | NAKED (C under NON_MATCHING) |
| `src/graphics/actor_part_1b208.c` | `UpdatePlatform`-`ClearPlatformMoverActive` (16) | matched |

Shared structs, externs and the virtual-call macros live in
`include/gobj_1a794.h`.

## What the code is

Found by scanning the ROM for Thumb pointers to each function; the method
tables are gcc 2.x C++ vtables (`{s16 this-adjust; pad; fn}` entries,
called through the `_call_via_r1`/`AD80`/`AD84`/`AD88` "call via
r1/r2/r3/r4" thunks), like issue #21's `input_ctrl`.

- **`CreateDingodileShieldCtrl`/`DestroyDingodile`/`CreateDingodile`**: constructor/destructor
  bodies of two subclasses of the `CreateBossCtrl` object family
  (`actor_part27.c`), method tables `gDingodileShieldVtable` and
  `gDingodileVtable` (`DestroyDingodile` is the latter's +0x4C destructor).
  `CreateDingodile` is called from `graphics_loading_21280.c`.
  `StartDingodileMotion` is `SetMegaMixMotionXFromSet`'s mirror-gated velocity copy, but taking
  its record index straight from `gDingodileMotionEntries` (8-byte `{a, b}`
  pairs into the 12-byte `gDingodileMotionRecords` vectors).
- **`struct gobj`** (0x80 bytes, method table `gPlatformVtable`):
  `CreatePlatform(id, x, y, index, kind)` allocates and constructs one (it
  inlines the constructor `InitPlatform`), looks its spawn record up through
  the level header at `*gEntityFlags` (u16 offset table at +8, records
  at +0xC), derives `type` (+0x78) from the record or forces it from `kind`
  (3/9-12 -> 4, 4 -> 2, 5 -> 3, 6 -> 6, 8 -> 7), and for types 1/5/6/7
  attaches a `struct mover` (type 6 uses `CreateCortexBossPlatformMover` instead when
  `GetBossIndex(gLevelState) == 1`). Callers: `trigger_effect.c`,
  `graphics_loading_21280.c`, `graphics_loading_21668.c`.
  - `CheckPlatformContact` (+0x0C) gates `ResolvePlatformCollision` on the player
    (`gPlayer`) being active and within 0x7FFF on both axes.
  - `ResolvePlatformCollision` resolves player-vs-object contact: two AABBs from
    `GetSpriteHitbox`, overlap via `AabbOverlaps`, then a classification into
    push-left/right (1/2), land-on-top (8) or hit-from-below (4) using the
    player's anim-record collision box (`anim_rec` +4..+9) and the
    `FindLineCrossing` edge probe; it then moves the player (`SetEntityPos`),
    sets `carried` (+0xAC) / `+0x68 = 8` when landing, and fires the
    player's method +0x68 (`_call_via_r4`) with event 0x0C/0x0F/0x10/0x11
    depending on the object type (the 3/4 variants gated on
    `IsBonusRoundDone`/`IsGemPathDone` and `gLevelState+0x8C`). Without
    overlap it only refreshes `carried` or clears the mover's `active`.
  - `UpdatePlatform` (+0x1C) steps or destroys the object and forwards to its
    mover; `sub_801B29C`/`sub_801B2A8` read/write bit 4 of +0x0D
    (`sub_801B29C` is called from `game_loop56.c`); `sub_801B2D8` clears
    bit 6 of +0x0C; `DestroyPlatform` is the destructor.
- **`struct mover`** (0x38 bytes, method table `gPlatformMoverVtable`,
  constructor `CreatePlatformMover`, destructor `DestroyPlatformMover`): an oscillating
  platform driver. `UpdatePlatformMover` (+0x0C) starts each axis with velocity
  record 1 of its `set` (12-byte records in `gPlatformMoverMotionRecords`,
  sign-flipped by `dirX`/`dirY`), accumulates the distance travelled and
  reverses once it exceeds `rangeX`/`rangeY` (twice the constructor's
  distance); kinds 5/6/7 add timed behaviour (see the function comment).
  `MovePlayerWithPlatform` drags the player along by the owner's per-frame
  displacement while `active`. `StartPlatformMoverMotionYFromSet`/`StartPlatformMoverMotionXFromSet` (+0x64/+0x5C)
  resolve a record and tail-call `StartCtrlTargetMotionY`/`StartCtrlTargetMotionX`.

UNUSED (no `bl`/`.4byte` in `asm/`, no C caller, no Thumb pointer in the
ROM): `SetDingodileStep`, `SetDingodileNextState`, `InitPlatform` (inlined instead),
`SetPlatformMoverMotionYFromSet`, `SetPlatformMoverMotionXFromSet`, `ClearPlatformMoverActive`.

## Matching notes

- **Bit clears/sets with the constant loaded before the `ldrb`**
  (`movs r1, #0x41; negs r1, r1; ldrb r2, [r0, #0xc]; ands`): plain
  `f &= ~0x40` folds to `#0xbf` after the load, and a bitfield loads
  first. Holding the mask in an `s32` local (`s32 m = ~0x40; f = m & f;`)
  reproduces it without asm; for OR the constant must be a `u8` local.
- **Reload CSE of small constants**: where the ROM rematerialises a mask
  (`movs r2, #0x11; negs`) right after using `1` or `0xF` in the same
  register, reload rewrites it as `subs r2, #0x12`. An empty
  `asm("" : "+r"(one))` on the earlier constant hides its value.
- **`CreatePlatformMover`'s stack-passed byte**: the ROM reads it with
  `add r0, sp, #0x18; ldrb r7, [r0]`. Only the address is computed in asm;
  the byte load is C.
- **`MovePlayerWithPlatform`** writes `p->hitAxes` through `&p->carried - 0x44` (the
  ROM reuses that address register), written that way explicitly.
- **`UpdatePlatformMover`**: gcc's `abs()` expands to a branch here; the ROM's
  `asr/eor/sub` is the in-place `ABS32` macro (as in `actor_part50.c`).
  The "mark actor gone" bitmap update is `InputCtrlStateDead`'s signed-division
  idiom. The rest is register pins (commented in the source).
- **`StartDingodileMotion`**: `index` pinned to r5 and kept live with an empty
  `asm("" : : "r"(index))` so the second lookup doesn't shift it in place.
- **`sub_801B29C`**: `(flags2 >> 4) & 1` (a bitfield read gives
  `lsl #27; lsr #31`).

## Parked: `ResolvePlatformCollision` (and, until the old_agbcc retry, `CreatePlatform`)

**`CreatePlatform` is matched now.** Built with old_agbcc, the reload
rotation described below comes out as in the ROM once every register pin
and the `rec` barrier are removed (with them left in, old_agbcc is still
off - 696 vs 700 bytes, first difference at +0x60). The spawn-record lookup is plain
`recs + offsets[index]`. Two workarounds remain: the hand-built
outgoing-argument block (old_agbcc also widens a `u8` stack argument to
`str`; the struct and `MOVER_NEW` moved to `include/mover_new.h`, shared
with `CreateCortexBossPlatformMover`), and the barrier on the palette nibble's `0xF`.

`ResolvePlatformCollision` stays NAKED: under old_agbcc its NON_MATCHING C is still
~290 diff lines off (1608 vs 1648 bytes). The first divergence is
structural rather than register choice - the ROM cross-jumps the
`x + w - x' + 1` / `y + h - y'` overlap computations of both branches
into one shared tail, where this source computes part of the sum before
the branch - and stripping the pins makes it worse, not better.

The agbcc-era analysis of both:

Both C reconstructions reproduce control flow, stack layout (including
`ResolvePlatformCollision`'s 0x44-byte frame and all six spill slots, which needed the
locals declared in slot order) and nearly every instruction. What's left
is reload's choice of scratch register: gcc 2.x `allocate_reload_reg`
walks the spill registers round-robin from `last_spill_reg`, so every
`mov rN, r8` base copy and every `movs rN, #c; str rN, [sp, #x]` lands in
a register decided by how many reloads came before it in the function.

- `ResolvePlatformCollision`: the rotation is two steps off from the first constant
  store on. The ROM loads the player's anim-record tag byte into r3 as a
  reload (so the next reload gets r0); this source's equivalent load is an
  ordinary pseudo, and pinning it to r3 fixes the instruction but not the
  rotation. Unpinned, `px`, `self`, `result` and `ty` also land in the
  ROM's registers except `px`, which needs a pin plus a pinned r0 load
  temp.
- `CreatePlatform`: 24 instructions still differ, all reload register choices
  (the spawn record lives in r8 and each use copies it to a low register).
  An exhaustive search over all 4096 on/off combinations of the 12
  register pins it uses (script-driven) bottomed out at 24; pinning
  individual copy sites moves the rotation elsewhere. This function also
  needs a hand-built outgoing-argument block for the `strb` of
  `CreatePlatformMover`'s 5th argument (this compiler always stores stack
  arguments as words): stores through `volatile` casts into a local struct
  at sp+0 and a call through a 4-argument function-pointer view. The
  spawn-record lookup needs `rec` to stay in the index register
  (`asm("" : "+r"(rec))`) to reproduce `add r8, r0`.

`ResolvePlatformCollision` is transcribed instruction-for-instruction as a `NAKED`
function for the matching build.

## Later pass: ResolvePlatformCollision matched (last-five NAKED retry)

`ResolvePlatformCollision` is now real C under old_agbcc (`actor_part_1ab98.o` joined
`OLD_AGBCC_OBJS`). An r8 hard-register hold up to the first overlap test
gives `result` r8 and `self` sb; both `FindLineCrossing` calls pass a
reassigned `px`; the player position goes through a `PosPtr` inline
instead of a `pp` local; the vtable call is an inline through the
method pointer. See [last5-naked-retry.md](last5-naked-retry.md).
