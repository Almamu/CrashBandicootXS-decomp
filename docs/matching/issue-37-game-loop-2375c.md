# Issue #37 follow-up: `PlayRoom` matched, `RunRoom` still raw

[docs/matching/issue-37-game-loop-234e8.md](issue-37-game-loop-234e8.md)
left this pair (`asm/code_3_2_17_2375c.s`, ROM `0x0802375C`-`0x08024007`)
completely untouched as "not yet confidently understood branch-by-branch."
This pass picks the smaller of the two back up.

## `PlayRoom` - matched (`src/system/game_loop39.c`)

A level-start dispatcher, called once from `UpdateGameFrame` when the
level object's own `+0xdc->+8` state field is `2`
(`asm/code_3_2_17_225a0.s`). It:

1. Fires two no-argument setup calls (`CreateEntitySpawner`, `ClearRoomExit`).
2. Allocates the whole per-level widget set: five `dual_array_manager`s
   (`gUnknown_030012E8`/`EC`/`F0`/`F8`/`F4`, the same struct
   `actor_part11.c`'s `InitPartList` already returns) and one
   `pool_manager` (`gCrateList`, `InitCrateList`'s own type from
   `actor_part12.c`), a generic 0x18-byte block (`gCamera`),
   the text-box singleton (`gLevelLayers`, lazily built by the
   still-raw `GetLevelLayers`), and the player actor itself
   (`gPlayer`, a 0x350-byte block handed to `InitPlayer`
   - the same constructor `actor_part77.c` already matched, called here
   with a genuine 5th stack argument the matched 4-parameter signature
   there simply never touches).
3. Sets the player's position from `self+0x10`/`0x14`
   (`SetEntityPos`), sets `player+0xc` bit 4, and mirrors `self+0x1c`
   bit 0 into `player+0x28` bit 4.
4. Reads the level-state record's (`self->0x18`) own `+8` "widget kind"
   field and dispatches on it (0/1/2) to construct one of three HUD
   counter/ring-buffer widgets, each via
   `InitActionCtrl`/`InitPlayerCtrl`/`CreateInputCtrl` (three different
   constructors, still raw) plus a `gStaticData_0816B92C`/`0816B934`/
   `0816B93C` action-table pointer stashed at `widget+4`
   (`SetCtrlAnimSet`). Widget kind `1` additionally builds an OAM entry via
   the standard `ResetSpriteFrameTimer`/`ResetSpriteFrameIndex`/`SetSpriteAnimDone` trio. All
   three cases finish by pointing `player+0x20`/`0x44` at the freshly
   built table/widget and firing `_call_via_r2` on it.
5. Unconditionally calls `RunRoom` (see below) and stashes its
   return value.
6. Tears the per-frame update queues back down: `DestroyLevelLayers` on the
   text-box singleton if non-NULL, `OperatorDelete` on the 0x18-byte
   block, a `_call_via_r2` call on the player object if non-NULL, then
   `DestroyPartList`/`DestroyCrateList` on each of the six widget-manager
   globals if non-NULL, and finally `DestroyEntitySpawner`.
7. Returns `RunRoom`'s result.

### Gotchas worth recording

- **The seven allocation stores need their destination address computed
  *before* the two calls that produce the value**, not after (the
  natural C evaluation order for a plain `global = fn(fn2(...))`
  assignment computes the address *after* the RHS). Fixed with the
  established `T **slot = &global; *slot = fn(fn2(...));` idiom - this
  reproduces the ROM's `ldr r4, =global` sitting *before* both `bl`s
  (with `r4` surviving them, since it's callee-saved) rather than a
  fresh `ldr r1, =global` computed afterward. The lone exception is the
  single-call `gLevelLayers = GetLevelLayers();` assignment, which
  the ROM computes address-after-value for (only one call in the way,
  so no register needs to survive it) - the plain, un-idiomed form
  reproduces that one directly.
- **`gPlayer`'s own address stays live in `r7` across the
  entire construction-and-dispatch section** (from its own allocation
  through all three switch cases), reused via repeated *fresh*
  `ldr r1/r2/r4, [r7]` reloads rather than ever being cached in a
  second local - by the time the function reaches the post-`RunRoom`
  teardown section, the ROM re-derives the address from scratch again
  (a fresh PC-relative literal, no `r7` left alive). Reproduced with one
  `void **d8 = &gPlayer;` local declared once near the top and
  referenced as `*d8` throughout construction/dispatch, while the
  teardown section refers to `gPlayer` by name again instead.
- **Every "fresh reload, do two field stores, then use the same pointer
  as a trailing call argument" case-block shape needs its *source
  value* computed before its *destination pointer*.** All three switch
  cases (and the `player+0xc`/`+0x28` bit-manipulation pair right before
  them) write to `player+0x20` and `player+0x44` (or similar) back to
  back; the ROM computes whatever's being stored (`**gSpriteBankSet`,
  a table pointer, etc.) *first*, then reloads `gPlayer`'s
  value into a register, then does both stores through it. A plain
  `u8 *pl = *d8; *(void**)(pl+0x20) = value;` computes `pl` first
  instead. Fixed by hoisting the value into its own local declared
  *before* the `pl` local in every such block.
- **`self` is pinned to `r8` for the entire function** (it has to
  survive dozens of `bl`s while r4-r7 are already busy with other live
  locals) via `register u8 *self asm("r8") = selfArg;`. Thumb can't
  address `[r8, #imm]` directly, so every field access needs its own
  `mov rN, r8` first - reproduced by wrapping each access in its own
  small block with a `register u8 *p asm("rN") = self;` local, matching
  the ROM's register choice at each individual site (mostly `r4`, once
  `r1`).
- **The `player+0xc |= 0x10` store** needs its *mask value* (`0x10`)
  loaded before the *current byte* is read, the opposite order a plain
  `*p |= 0x10;` compiles to. Fixed the same way as the case-block
  gotcha above, with explicit register-pinned locals in the ROM's exact
  order (`val` before `cur`).
- **The `player+0x28` bit-clear/set mask (`& ~0x10 | bit`)**: written
  as `~0x10`, this compiler constant-folds the whole mask to a single
  immediate load - one instruction short of the ROM's real `movs r0,
  #0x11 / rsbs r0, r0, #0` runtime negation. The established
  "negative-constant bit-clear idiom" (`docs/matching.md`,
  `sub_800A70C` in `actor_part14.c`) fixes it, but *only* when the
  negative literal (`-0x11`) is bound to a `register ... asm("r0")`
  local first - a bare `*p & -0x11` inline still constant-folds despite
  being the "right" literal, since nothing forces the compiler to treat
  it as a runtime value rather than a compile-time one. This one 2-byte
  gap is also why the function's overall size looked 4 bytes short
  during isolated compiles: the missing instruction plus its knock-on
  4-byte alignment padding before the following literal pool.
- **The `bit = (self->0x1c & 1) << 4` computation's operand order
  matters**: the ROM loads the literal `1` into `r1` *before* reading
  `self->0x1c` into `r4` (reusing the same register the pointer copy
  was just in, i.e. `ldrb r4, [r4, #0x1c]`), not after. A plain
  `(self[0x1c] & 1) << 4` computes the load first. Fixed by writing
  `(one & rawbit) << 4` with `one` a `register u32 asm("r1") = 1;`
  local declared *before* the register-pinned `self[0x1c]` read.
- **A redundant `self`-from-`r8` reload right before the mode fetch**:
  the ROM re-issues `mov r4, r8` immediately after the bit-manipulation
  block above, even though `r4` already holds that exact value and
  nothing has clobbered it - this compiler's own value tracking
  otherwise elides the second `mov`. An `asm volatile("" ::: "r4");`
  barrier between the two register-pinned blocks forces the reload
  back in, matching the ROM's (mildly wasteful) instruction count.
- **The 3-case switch on `mode` (0/1/2) compiles to a binary-search
  comparison tree** (`cmp mode,1; beq case1; cmp mode,1; bgt (>1 ?
  case2 : default); cmp mode,0; beq case0; else default`), not a linear
  if/else-if chain or a jump table - reproduced directly by writing a
  plain `switch (mode) { case 0: ...; case 1: ...; case 2: ...; }` and
  letting this compiler's own switch lowering pick that shape, rather
  than hand-writing the comparison order.
- **`InitPlayer`'s 5th argument**: the matched 4-parameter signature in
  `actor_part77.c` never reads a 5th argument, but this call site (and,
  per that file's own doc comment, `CreateSpriteObj` elsewhere in the same
  neighborhood) passes one anyway - a real stack argument (`str r2,
  [sp]` sitting *before* the register arguments are even fully loaded,
  reusing whichever register already held `0`). Declared here with an
  unprototyped `extern void *InitPlayer();` so the call `InitPlayer(ptr,
  0xffff, 0, 0, 0)` can pass the extra trailing `0` the ROM's own
  (differently-prototyped, in this translation unit) declaration
  allowed.

## `RunRoom` - still raw

The ~650-instruction jump-table-driven continuation this function calls.
Its real bytes now live in `asm/code_3_2_17_23a1c.s` (renamed from
`asm/code_3_2_17_2375c.s`, since that file no longer starts at
`0x0802375C`). Left untouched for this pass - see
`docs/rom_map.md`'s "Traced the fade-to-black's trigger" and "Resolved:
`IsRoomExitRequested`'s gate" sections for what's already understood about its
6-case jump table, the `gLevelTable` per-level table it indexes,
and its wait-loop/fade/post-fade structure. `AddPaletteCycle`'s exact
6-argument call shape (self, targets, lists, angle, list_count,
direction - matched in `hud_icon_slot.c`) is now confirmed against all
four of this function's own call sites, closing one of the previously
open questions; the `gThemePaletteCycle1A`/`0816C830`/`0816C842`/
`0816C862` tables it points at are still just `u16*`/opaque data,
their record shape not derived. A good next target for a dedicated pass.

See [docs/status/game_loop.md](../status/game_loop.md) for the updated
matched/parked/raw lists.

## `RunRoom` - closed, parked `NAKED` (dedicated follow-up pass)

Full branch-by-branch trace, confirming and extending everything the
previous pass above left open.

### The 6-case dispatch map

Opening: index `gLevelTable` by `self+0` (the confirmed
36-slot, 0x24-byte-stride per-level master table - `settings_menu19.c`/
`oam_count.c`/`game_loop17.c` all have their own struct view of it).
Read its `+0x1c` byte (`isBoss`, first read as an "initialized" guard; calls `CheckAllCratesBroken` once if
still clear), feed its `+0x14`/`+0x18` fields (`maskAssistDeaths`/`crateAssistDeaths`) straight through to
`SetMaskAssistDeaths`/`sub_8023110`, then dispatch on its `+4` field
(`state - 1`, clamped `[0,5]`; `state == 0` or `state > 6` takes the
`default` path):

| jump-table index | `state` | code |
| --- | --- | --- |
| 0 | 1 | `_08023B20` (shared with index 5) |
| 1 | 2 | `_08023AF4` |
| 2 | 3 | `_08023B60` |
| 3 | 4 | `_08023BC4` (== `default`) |
| 4 | 5 | `_08023B8C` |
| 5 | 6 | `_08023B20` (shared with index 0) |

Every non-default case resets `gPaletteCycles` (the `palette_cycler`
`hud_icon_slot.c` documents) via `ClearPaletteCycles`, then fires
`AddPaletteCycle(queue, targets, lists, angle, list_count, direction)`:

- **state 1 or 6**: *two* calls -
  `AddPaletteCycle(queue, (u16 *)0x05000000, gThemePaletteCycle1A, 0x10, 9, 0)`
  then `AddPaletteCycle(queue, (u16 *)0x05000000, gThemePaletteCycle1B, 0x14, 9, 0)`.
- **state 2**: `AddPaletteCycle(queue, (u16 *)0x05000000, gThemePaletteCycle2, 6, 5, 1)`
  (the only case with `direction=1`).
- **state 3**: `AddPaletteCycle(queue, (u16 *)0x05000000, gThemePaletteCycle3, 0xa, 0x10, 0)`.
- **state 5**: `AddPaletteCycle(queue, (u16 *)0x05000000, gThemePaletteCycle5, 0x14, 5, 0)`.
- **default** (state 0/4/`>6`): no reset, no `AddPaletteCycle` call - just
  clears the queue's own `active` byte directly.

`(u16 *)0x05000000` is GBA palette RAM itself, passed straight through
as the queue's `targets` argument - this is a **palette color-cycle
animation**, reusing the exact same generic rotate-by-index-list engine
`TickPaletteCycles`/`AddPaletteCycle`'s other (HUD-digit) call sites drive, not a
new mechanism.

**Resolves the open table-shape question**: `gThemePaletteCycle2`
(5 entries), `0816C81E` (9), `0816C830` (9), `0816C842` (0x10), and
`0816C862` (5) are not per-level records - they're plain, tightly
packed `u16[]` "permutation index list" arguments to `AddPaletteCycle`'s
own `lists` parameter. Confirmed via address deltas exactly matching
each call site's own `list_count` argument (`0816C814`->`0816C81E`:
0xA = 5*2; `0816C81E`->`0816C830`: 0x12 = 9*2; `0816C830`->`0816C842`:
0x12 = 9*2; `0816C842`->`0816C862`: 0x20 = 0x10*2) - no gap, no
padding, no further struct needed.

**A genuine compiler-internal code-sharing quirk, not a modeling
error**: state 5's setup falls straight into label `_08023BA6`
(`mov r3, #0x14; bl AddPaletteCycle; b _08023BCC`), and state 1/6's *second*
`AddPaletteCycle` call (after its first call has already completed and its
second call's `targets`/`list_count` args are already loaded into
`sb`/`r8`) jumps into that *same* physical label mid-setup, rather than
having its own separate `angle=0x14`/`bl`/`b` copy. Both calls share
`angle=0x14, direction=0`; only the `lists` pointer and `list_count`
differ, and the compiler evidently folded the two calls' identical
tails into one block. This is cross-jump-table-target block sharing,
not a within-one-switch-statement `goto`-fallthrough shape - out of
reach for this project's usual `goto`-restructuring technique, which
targets exactly the latter.

### Shared tail, wait loop, and post-fade

The 5 non-default cases (plus the default's direct clear) converge on
one tail: `SetupRoomBlend(self)`/`ResetObjBuffers()`, then a widget-kind check
(`self->0x18->+8`, the same field `PlayRoom` dispatched its own
widget-construction switch on) that - if `1` - re-stamps the player's
`+0x2d` byte to `0x1f` and refreshes its OAM entry
(`ResetSpriteFrameTimer`/`ResetSpriteFrameIndex`/`SetSpriteAnimDone`), then unconditionally
recomputes the player's `+0x29` low nibble from `GetSpriteAnimPaletteSlot(player)`
(the established negative-constant bit-clear idiom) and fires
`LoadPaletteSlot` against the tile-asset cache using a `player+0x20`-table
lookup indexed by `player+0x2d * 7` (0x1c-byte stride), then flushes
`gCamera` (`SnapCamera`) and the text-box singleton
(`ResetLevelLayers`).

If the widget kind is `0`: probes `IsInBonusRound`/`IsInBonusRoom` or
`IsInGemPath`/`IsInGemPathRoom` (level-object and self readiness checks);
on success, clears the player's busy bit 7, re-stamps `+0x2d` to
`0x29`, refreshes the OAM entry again, plays a sound effect
(`gAudioContext` as sample id, priority `0x2c`, via `PlaySfx`),
fires the `player+0x44`-table's `_call_via_r2` trampoline (mode
`0x29`), repeats the same `LoadPaletteSlot` tile-cache call, and pings
`gHud` (`ShowHudCounters`).

Either way: flushes the four HUD ring-buffer managers (`CullPartList`
on `030012F4`/`EC`/`F0`/`F8`), a `UpdateRoomFrame(self)` refresh, and the
fade-cluster `SetDispcntMode(0)`/`ShowObj`/`CommitDispcnt`/`CommitBlendRegs`
reset quartet, landing at the **wait loop** (confirming and completing
`docs/rom_map.md`'s earlier trace): poll `IsRoomExitRequested` each iteration;
while not ready and the player's `+0xc` bit 0 is clear, run one more
pass (`ResetObjBuffers`/`UpdateRoomFrame`, a `RunPauseMenu` input-driven mini-
dispatch that can early-exit the whole function with return value `1`
or `2` after firing `ResumeRoomAfterPause`'s level-end teardown, a
`gKeys` input-flag-gated `ShowHudCounters` ping, `UpdatePartList`
on three ring-buffer managers, two `_call_via_r1` trampoline probes
against the player's own `+0x18`/`+0x38`/`+0x18` tables, `UpdateCrateList`
on `gCrateList`, `UpdateHudSlides`, and a `gLevelState+0x8c`-
gated `TickLevelClock` call) before looping back. Once ready: fires the
fade (`FadePaletteToBlack`).

**Post-fade** (converging at `_08023F92`): sets the return value to
`0`, tries two `SetCheckpoint` "spawn" dispatches gated by
`IsInBonusRoom`/`IsInBonusRound`/`GetBonusPlatform` or
`IsInGemPathRoom`/`IsInGemPath` (both skip straight to the flush tail on
failure); falling through both, loops `gCrateList` counting
entries whose `_call_via_r1` trampoline probe returns `3` *and* whose
own `+0x4e` tag is `0xa` (the physics-subsystem state tag
`gCrateHitResponse` indexes,
[docs/matching/issue-12-physics-collision.md](issue-12-physics-collision.md)),
then calls `AddPendingSwitchCrates(gLevelState, count)`.

**Final tail** (every path converges here): flushes all five hot IWRAM
widget-manager globals (`ClearPartList` on `030012E8`/`EC`/`F0`/`F8`/`F4`,
`ResetCrateList` on `0300130C`), resets the fade cluster's own bitfield
accessors (`HideBg0`/`HideBg1`/`HideBg2`/`HideBg3`/
`HideObj`/`WaitForVBlank`/`CommitDispcnt`), and returns `sl` - `1` by
default, `2` from the wait-loop's `RunPauseMenu`-driven early exit, or
`0` once the post-fade branch was reached. `PlayRoom` itself stashes
and returns this value unmodified.

### Matching result: parked `NAKED`

Not attempted as real C. Beyond the ~650-instruction size (already
past this project's demonstrated C-reconstruction ceiling for
`game_loop`-neighborhood jump-table functions - `UpdateGameFrame`,
`sub_8017AB0`), the state 1/6 <-> state 5 shared-tail quirk documented
above is a *cross-jump-table-target* code-sharing decision, not the
*within-one-switch* `goto`-fallthrough shape this project's toolbox is
built to reproduce - forcing it from plain C would mean fighting the
compiler's own switch lowering rather than working with it, for a
function already well past the size where that's been worth trying.

Transcribed instruction-for-instruction into `src/system/game_loop56.c`
(new file - `asm/code_3_2_17_23a1c.s` is now gone entirely), keeping
the ROM's own `_0XXXXXXX` hex-address labels verbatim as file-local asm
symbols, the same convention used throughout this project's other
NAKED transcriptions. `ldscript.txt` swaps the retired `.o` entry for
`game_loop56.o` in place. Verified via a full clean
`make NON_MATCHING=1 report` (no warnings for this file) and a full
clean `make compare` (`crashbandicootxs.gba: La suma coincide`).
**This closes GitHub issue #37.**

## Later pass: hard-register hold

`RunRoom` is now real C under old_agbcc; `game_loop56.o` joined
`OLD_AGBCC_OBJS` (it is the file's only function). `struct fx_direction`
gained a zero-length array member, which makes it BLKmode: the compound
literal is then stored straight into the outgoing stack slot, address
first, as in the ROM (12 halfwords). An r0/r1 hold replaces the old r2
pin in the post-fade position copy (2 halfwords). See
[hard-register-hold-retry.md](hard-register-hold-retry.md).
