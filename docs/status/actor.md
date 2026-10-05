# Status: actor

The per-instance actor "self" object family - `struct actor` and its
many satellite files (`src/graphics/actor_part*.c`,
`src/util/aabb_setup.c`). Filed under `src/graphics/` on disk
(the ROM's actor code lives interleaved with rendering code, and
several actor functions are themselves OAM/sprite-draw routines), but
tracked as its own `actor` category here since `docs/rom_map.md` and
the `decomp-chunk` issue generator both treat it as a distinct system
from "core" graphics.

## Matched

- **Issue #9-#11 box/collision NAKED retry** ([docs/matching/issue-9-11-box-naked-retry.md](../matching/issue-9-11-box-naked-retry.md)):
  `CheckSpritePickup` (`sprite.c`), `UpdatePartList` (`sprite_anim.c`),
  `InitCrateList` (`part_list.c`), `ResetCrateList` (`crate_list_reset.c`),
  `sub_8009BE0` (`step_probe.c`) and `ProbeGroundSpriteTerrain`/`ProbeGroundSpriteFloor`
  (`ground_sprite_collide.c`) are real C now; they were NAKED. `sprite.o`
  and `ground_sprite_collide.o` moved to old_agbcc (whole-file matches).

- **Issues #15/#16/#17 second NAKED retry** ([docs/matching/issue-15-16-17-naked-retry-2.md](../matching/issue-15-16-17-naked-retry-2.md)):
  the action-table handlers `ActionCtrlStateCrawl` (`action_ctrl_states.c`, file moved
  to old_agbcc), `ActionCtrlStateLand` (`action_ctrl_land.c`, both compilers) and
  `TryActionCtrlDoubleJump` (`action_ctrl_update.c`, old_agbcc) - plain C on
  `include/action_obj.h`'s `struct act`, no pins or barriers.
- **Issues #48/#49/#52 NAKED retry** ([docs/matching/issue-48-49-52-aabb-naked-retry.md](../matching/issue-48-49-52-aabb-naked-retry.md)):
  the AABB-overlap group `PolarIsTouchingPlayer`, `JetpackIsTouchingPlayer`, `FindShotTarget`
  (`actor_part103.c`) and `DetonateNearbyPolarNitros` (`actor_part19h.c`) - one shared
  inline with the three boxes in one frame struct, both files moved to
  old_agbcc; the tile-map fill `FillCellAnimTilemap` (`actor_part98.c`) and its
  inlined twin in `ResetCellAnimBg` (`actor_part95.c`) - `tile++` in each
  branch; `UploadCellAnimFrame` (`actor_part95.c`); and the trampolines
  `JetpackIsPauseLocked`/`PolarIsPauseLocked` (`actor_part94.c`), which return the
  callee's result.

- **Actor-zone NAKED near-miss retry** ([docs/matching/actor-zone-naked-retry.md](../matching/actor-zone-naked-retry.md)):
  the AABB-overlap trio `UpdateYeti` (`actor_part74.c`), `IsTouchingYeti`
  (`actor_part75.c`) and `IsTouchingAirship` (`airship_touch.c`) - the boxes
  are members of one stack-frame struct, so their addresses are
  rematerialized from sp as in the ROM; `LoadYetiGraphics`
  (`actor_part75.c`); the BG-tilemap blit twins `DrawAirshipMap`
  (`airship_map.c`) and `DrawHovercraftMap` (`hovercraft.c`); and the
  easing helper `SteerAirship` (`airship.c`). All plain C, no
  register pins; they were NAKED. `airship_map.c`, `airship_touch.c`,
  `actor_part75.c` and `hovercraft.c` moved to old_agbcc.

- **`ActionCtrlStateIdle`** (`src/player/action_ctrl_idle.c`) and **`HandleActionCtrlAirInput`**
  (`src/player/action_ctrl_update.c`) - issue #16: two
  `gActionCtrlStateTable` action-table helpers on the player/action object
  (`include/action_obj.h`). Plain C under old_agbcc (both files moved to
  `OLD_AGBCC_OBJS`); they were NAKED transcriptions. See
  [issue-15-16-naked-retry.md](../matching/issue-15-16-naked-retry.md).
- **Issues #58/#61 NAKED retry** ([docs/matching/issue-58-61-naked-retry.md](../matching/issue-58-61-naked-retry.md)):
  the boss-weapon cluster's `AirshipStateFireballs` (`airship_states.c`),
  `AirshipStateCannon` (`airship_states.c`), `AirshipStateExplode` (`airship_explode.c`),
  `CreateAirship` (`airship.c`), `SpawnAirship` (`airship.c`),
  `UpdateAirship` (`airship.c`), `LoadAirshipGraphics` (`airship_load_graphics.c`),
  and the `gHovercraft` singleton's `HovercraftStateCloseIn`, `HovercraftStateFallBack`,
  `CreateHovercraft`, `SpawnHovercraft`, `UpdateHovercraft`, `LoadHovercraftGraphics`
  (`hovercraft.c`). Plain C under current agbcc, no register pins;
  they were NAKED.
- **`InitContinuePromptGraphics`** (`src/menus/continue_prompt.c`) - issue #63: the fade
  overlay's other setup half (icon manager hookup, tile-cache seeding
  loop). Plain C, built with old_agbcc; it was raw asm
  (`asm/code_3_2_20_28568_c99c_31784_33ef4_3487c.s`, now removed). See
  [old-agbcc-round5.md](../matching/old-agbcc-round5.md).
- **`DrawJetpackCollectedWumpa`** (`src/bosses/hovercraft.c`) - issue #60's last
  function, a bounding-box-culled sprite draw (`DrawActor`'s
  shape with the scale flag fixed at 0). Plain C; it was NAKED. See
  [issues-14-53-60-last-naked.md](../matching/issues-14-53-60-last-naked.md).
- **`UpdatePolarElectricFence`** (`src/graphics/actor_part126.c`) - issue #53's last
  function, a hazard/proximity state machine that tests the part's own
  box and three `gStaticData_0817A7xx` boxes. Plain C; it was NAKED. See
  [issues-14-53-60-last-naked.md](../matching/issues-14-53-60-last-naked.md).
- **Issue #9 NAKED retry** ([docs/matching/issue-9-naked-retry.md](../matching/issue-9-naked-retry.md)):
  `GetSpriteBounds`/`GetSpriteHitbox` (`sprite.c`), `AdvanceSpriteAnim`
  (`sprite.c`), `CollidePartList`/`CollidePartWithPlayer` (`sprite_anim.c`),
  `CollidePartWithObject` (`part_collide.c`), `CollideCrateGrid` (`crate_grid_collide.c`),
  `CollideCrateGridPartWithPlayer` (`crate_grid_collide.c`), `CollidePlayerWithCrates` (`crate_player_collide.c`),
  `CollideCrateGridPartWithObject` (`crate_list.c`) - all under old_agbcc (the whole
  objects moved to `OLD_AGBCC_OBJS`), mostly by passing the collision
  box by value - and `PlayerHasRoomForAnim` (`player_anim_room.c`, either compiler,
  guarded do-while list walk). Previously parked as NAKED below.

- `src/objects/sprite.c` (new file - `DrawSpriteAt`'s real ROM
  address isn't adjacent to `graphics.c`'s matched functions, since
  `DrawAffineSpritePieces` sits unclaimed between them; see
  `docs/matching.md`): `DrawSpriteAt`, `DrawSprite`, `DestroySpriteRenderer`,
  `nullsub_2`, `ResetSpriteObj`
- `src/objects/sprite.c` (new file - `GetSpriteAttackBox`'s real ROM
  address isn't adjacent to `sprite.c`'s matched functions either,
  since the parked `GetSpriteBounds`/`GetSpriteHitbox` sit raw between them;
  see `docs/matching.md`): `GetSpriteAttackBox`, `GetSpriteBodyBox`. (This file's
  `CheckSpritePickup` is a NAKED transcription tracked as parked, not matched
  - see below and `docs/matching/naked-sub_8007dbc.md`.)
- `src/objects/sprite.c` (new file - directly adjacent to
  `sprite.c`'s matched functions now that `CheckSpritePickup` is
  byte-exact too, closing the old raw gap between them):
  `IsSpriteObjOnScreen`, `SpriteObjOverlapsRect`
- `src/objects/sprite_obj.c` (new file, now directly adjacent to
  `sprite.c`'s matched functions): `SpriteHitboxOverlaps`, `GetSpriteAnimPaletteSlot`,
  `sub_8008188`, `sub_8008200`, `sub_8008278` (the latter three
  originally a NAKED transcription, matched to real C in a later
  session - see `docs/matching.md`'s "Parked, not matched: sub_8008188"
  entry and its "Update" note)
- `src/objects/sprite_obj.c` (new file, now directly adjacent to
  `sprite_obj.c`'s matched functions): `IsSpriteObjInsideRect`,
  `IsSpriteObjNearCamera`, `ApplySpriteObjVelocity`, `DrawSpriteObj`, `UpdateSpriteObj`,
  `GetSpriteObjHitbox`, `GetSpriteTileBase`, `GetSpriteFrame` (the latter originally a
  NAKED transcription, matched to real C in a later session - see
  `docs/matching.md`'s "Parked, not matched: GetSpriteFrame" entry and its
  "Update" note)
- `src/objects/sprite_obj.c` (new file, now directly adjacent to
  `sprite_obj.c`'s matched functions): `GetSpriteObjPriority`, `CreateSpriteObj`, `GetSpriteObjClassId`,
  `DestroySpriteObj`, `InitSpriteObj`, `GetSpriteFrameAnchor`, `GetSpriteFrameThirdBox`,
  `GetSpriteFrameAttackBox`, `GetSpriteFrameBodyBox`, `GetSpriteAnimRecord`, `SetSpriteFrameIndex`,
  `GetSpriteScreenSpace`, `SetSpriteScreenSpace`, `IsSpriteHidden`, `ToggleSpriteHidden`,
  `IsPartSolid`, `ClearPartSolid`, `SetPartSolid`, `IsSpriteObjVulnerable`,
  `ClearSpriteObjVulnerable`, `SetSpriteObjVulnerable`, `ResetSpriteAnimIndex`, `IsSpriteObjCollisionEnabled`,
  `DisableSpriteObjCollision`, `EnableSpriteObjCollision`, `GetSpriteAnimating`, `SetSpriteAnimating`,
  `SetSpriteFlipX`, `SetSpriteFlipY`, `SetSpriteAnimDone`, `GetSpriteAnimPaletteId`,
  `GetSpritePalette`, `SetSpritePalette`, `SetSpriteAnimTable`, `GetSpriteAnimTable`,
  `IsSpriteAnimLooping`
- `src/objects/sprite_anim.c` (new file, now directly adjacent to
  `sprite_obj.c`'s matched functions): `GetSpriteAnimFrameCount`, `GetSpriteAnimDuration`, `ResetSpriteFrameIndex`,
  `SetSpriteFrameTimer`, `ResetSpriteFrameTimer`, `SetSpriteAnimIndex`, `SetSpriteAnim`,
  `IncSpriteFrameIndex`, `IncSpriteFrameTimer`, `SetSpriteMoveAxes`, `GetSpriteMoveAxes`,
  `GetSpriteFrameIndex`, `GetSpriteFrameTimer`, `GetSpriteAnim`, `GetSpriteGfxMode`,
  `SetSpriteGfxMode`, `GetSpriteFlipX`, `GetSpriteFlipY`, `GetSpriteAnimDone`,
  `GetSpriteMosaic`, `GetSpriteOamPalette`, `GetSpriteColorMode`, `GetSpriteAffine`,
  `SetSpriteAffine`, `DrawSpriteWithOffset`, `SetSpritePriority`, `GetSpritePriority`,
  `DestroyUiSpriteObj`, `InitUiSpriteObj`

- `src/objects/part_list_cull.c` (new file, now directly adjacent to
  `sprite_anim.c`'s matched functions): `CullPartList`, `ClearPartList`, `CollidePartsOfClass`

- `src/objects/part_list.c` (new file, now directly adjacent to
  `part_collide.c`'s `CollidePartWithObject` range): `DrawPartList`, `RemoveFromPartList`, `RemovePartListAt`,
  `AddToPartList`, `DestroyPartList`, `InitPartList`. (This file's `InitCrateList`,
  compiled right after these, is a NAKED transcription tracked as
  parked, not matched - see below and
  `docs/matching/naked-sub_8008f20-ResetCrateList-freelist.md`.)

- `src/crates/crate_grid_unlink.c`/`crate_list_update.c`/`crate_grid_collide.c`/`crate_player_collide.c` (new files, NAKED-transcription-
  only - see "Parked - NAKED transcription" below for what each one
  holds): `UnlinkCrateFromGrid`, `UpdateCrateList`, `CollideCrateGrid`, `CollideCrateGridPartWithPlayer`,
  `CollidePlayerWithCrates` respectively, each dropped into `ldscript.txt` between
  the remaining `asm/code_3_2_13*.s` guard splits at its own real ROM
  address

- `src/crates/crate_grid_link.c` (new file - `LinkCrateToActiveBucket`'s real ROM
  address sits between the NAKED `UnlinkCrateFromGrid` (`crate_grid_unlink.c`) and
  `UpdateCrateList` (`crate_list_update.c`), replacing the retired
  `asm/code_3_2_13_9150.s` guard; named `crate_grid_link.c` since
  `CollideCrateGrid`'s own NAKED conversion claimed `crate_grid_collide.c` first):
  `LinkCrateToActiveBucket` - see
  `docs/matching/LinkCrateToActiveBucket-loop-invariant-hoist-matched.md`

- `src/crates/crate_list_draw.c` (new file - `DrawCrateList`'s real ROM
  address isn't adjacent to `part_list.c`'s matched functions,
  since the NAKED `InitCrateList` and `UnlinkCrateFromGrid`/`UpdateCrateList` plus
  the now-matched `LinkCrateToActiveBucket` (`crate_grid_link.c`) sit between them;
  named `crate_list_draw.c` since `crate_grid_collide.c`/`crate_grid_link.c`
  were both already claimed by the time this landed): `DrawCrateList`

- `src/crates/crate_list_reset.c` (new file, NAKED-transcription-only,
  `ResetCrateList`'s real ROM address isn't adjacent to `part_list.c`'s
  own functions - see `docs/matching/naked-sub_8008f20-ResetCrateList-freelist.md`.
  Named "i" - `CollideCrateGrid` claimed `crate_grid_collide.c`, the now-matched
  `LinkCrateToActiveBucket` claimed `crate_grid_link.c`, and the now-matched
  `DrawCrateList` claimed `crate_list_draw.c`, all in parallel PRs merged
  first), dropped into `ldscript.txt` in place of the retired
  `asm/code_3_2_13_9914.s`

- `src/crates/crate_list.c` (new file - `RemoveCrateFromList`'s real ROM
  address isn't adjacent to `part_list.c`'s matched functions
  either, since the NAKED `InitCrateList`, the now-matched `LinkCrateToActiveBucket`
  (`crate_grid_link.c`) and `DrawCrateList` (`crate_list_draw.c`), the NAKED
  `UnlinkCrateFromGrid`/`UpdateCrateList`/`CollideCrateGrid`/`CollideCrateGridPartWithPlayer`/`CollidePlayerWithCrates`/`ResetCrateList`,
  all sit between them; see `docs/matching.md`):
  `RemoveCrateFromList`, `RemoveCrateListAt`, `AddCrateGridNode`, `LinkCrateInGrid`,
  `AddCrateToList`, `DestroyCrateList`

- `src/objects/step_probe.c` (new file, NAKED-transcription-only -
  `sub_8009BE0`, see "Parked - NAKED transcription" below)

- `src/objects/player_contact.c` (new file - `CheckPlayerContact`'s real ROM
  address isn't adjacent to `crate_list.c`'s matched functions
  either, since NAKED `sub_8009BE0` (`step_probe.c`) sits between
  them; see `docs/matching.md`): `CheckPlayerContact`

- `src/objects/moving_sprite.c` (new file - `ApplySpriteVelocity`'s real ROM
  address isn't adjacent to `code_3_2_15.o`'s raw content either, since
  NAKED `sub_8009BE0` (before that) was already handled separately; see
  `docs/matching.md`):
  `ApplySpriteVelocity`, `SetSpritePrevPos`, `GetSpritePrevPos`, `GetSpritePrevY`,
  `GetSpritePrevX`, `GetMovingSpriteClassId`, `CreateMovingSprite`, `DestroyMovingSprite`,
  `ResetMovingSprite`, `InitMovingSprite`, `UpdateMovingSprite`
- `src/objects/moving_sprite_collide.c` (new file - `HitMovingSprite`'s real ROM
  address is adjacent to `moving_sprite.c`'s matched functions; see
  `docs/matching.md`): `HitMovingSprite`, `ClassifySpriteContact`, `CollideMovingSprite`,
  `GetGroundSpriteHitMask`, `HasGroundSpriteHitMask`, `ClearGroundSpriteHitMask`, `AddGroundSpriteHitMask`,
  `SetGroundSpriteHitAxes`, `GetGroundSpriteHitAxes`, `SetSpriteSpeedY`, `SetSpriteSpeedX`,
  `GetSpriteSpeedX`, `GetSpriteSpeedY`, `GetSpriteCtrl`, `AttachSpriteCtrl`,
  `StartSpriteMotionY`, `SetSpriteMotionY`, `StartSpriteMotionX`, `SetSpriteMotionX`,
  `GetGroundSpriteProbeTries`

- `src/objects/ground_sprite_collide.c` (issue #9/#10 follow-up - see
  [docs/matching/issue-9-0x0800a178-graphics.md](../matching/issue-9-0x0800a178-graphics.md)):
  `CollideGroundSprite` - the part-object physics dispatcher, the sole caller
  of `ProbeGroundSpriteTerrain` (both now share this file). Was left raw the first
  pass since its own gate logic calls `sub_8009BE0`, then still-
  unresolved; `sub_8009BE0` is now fully understood (see "Parked -
  NAKED transcription" below), which was enough to close this one as
  real, byte-exact matched C. Returns `self+0x68` (a persistent,
  cumulative per-object collision-axis mask, distinct from
  `ProbeGroundSpriteTerrain`'s own per-call `self+0x74` scratch mask) unchanged
  unless `self+0xc` bit 7 is set; if set, calls `ProbeGroundSpriteTerrain` and OR's
  its result into `self+0x68`, fires the already-matched `CollideMovingSprite`
  trampoline, then - if `self+0x68` bit 3 (the Y-axis bit) is now set -
  clears `self+0xc` bits 0/5 and, unless `self+0xd` bit 1 is already
  set, cross-checks the Y-axis hit via a second, independent
  `sub_8009BE0(self, 8, quad)` step-probe, rolling `self+0xc` bit 5 in
  and `self+0x68` bit 3 back out if that probe doesn't also confirm it.
  Needed direct register pinning at several points to reproduce the
  ROM's exact register choices (a `flags`/`bit7` pair for the leading
  gate test, the established negative-constant-mask idiom for the
  `&= ~0x21` clear, a 4-register chain for the `(self+0xd>>1)&1` gate,
  `CollideMovingSprite`'s own addr-before-fn trampoline ordering reused for
  `self->table+0x10/0x14`, and two more mask/byte/result pairs for the
  trailing `|= 0x20`/`&= 7` writes). Retires `asm/code_3_2_11.s`
  entirely (it held only this function).

- `src/objects/ground_sprite.c` (new file - `DrawGroundSprite`'s real ROM
  address isn't adjacent to `moving_sprite_collide.c`'s matched functions
  either, since a raw/parked span (`ProbeGroundSpriteTerrain`-`sub_800A590`,
  `ProbeGroundSpriteTerrain`/`ProbeGroundSpriteFloor` NAKED-parked but `UpdateGroundSprite`/
  `sub_800A590` matched in `ground_sprite_update.c`) sits between them; see
  `docs/matching.md`): `DrawGroundSprite`, `GetGroundSpriteClassId`,
  `CreateGroundSprite`, `DestroyGroundSprite`, `ResetGroundSprite`, `InitGroundSprite`,
  `IsGroundSpriteGrounded`, `ClearGroundSpriteGrounded`, `SetGroundSpriteGrounded`, `IsGroundSpriteFloorProbeEnabled`,
  `DisableGroundSpriteFloorProbe`, `EnableGroundSpriteFloorProbe`, `ClearSpriteObjFlag5`, `SetSpriteObjFlag5`,
  `GetSpriteObjFlag5`, `GetMovingSpriteCtrl`

- `src/objects/ground_sprite_update.c` (new file, GitHub issue #9): `UpdateGroundSprite`/
  `sub_800A590` - a moving-platform "ride along" hookup, nudging `self->y`
  by the delta between a cached and current position-record lookup.
  Matches the ROM's register roles for `self`/the record pointer
  directly (`r4`/`r3`); the remaining gap (a genuine fifth scratch
  register, `r5`, just to hold an offset immediate for the record's
  `+2`/`+5` field reads, which no C-level phrasing alone ever made this
  compiler introduce) closed by materializing the ROM's own load
  sequence directly via `asm volatile`. Retires the raw
  `asm/code_3_2_11_a528.s`. See
  [docs/matching/issue-9-0x08007634-actor.md](../matching/issue-9-0x08007634-actor.md).

- `src/player/player_reset.c` (GitHub issue #9): `ResetPlayer` - a
  part-object velocity/state reset+constructor that hooks up a child
  object at `self+0xb0` (closed a gap an earlier session parked on -
  needed interleaved running-pointer cursors, several register-pinned
  idioms, and an inline-asm-anchored instruction order in a few spots)
  - and `ResetPlayerForRoom` - a part-object velocity/state reset that
  dispatches a sub-state byte to one of three teardown helpers; see
  [docs/matching/issue-9-0x08007634-actor.md](../matching/issue-9-0x08007634-actor.md).

- `src/player/player_update.c`/`src/player/player_flags.c` (new
  files, split around the raw untouched `InitPlayer` - see
  `docs/matching.md`): a new not-yet-named big object's accessors -
  `HasPlayerRampYTarget`, `ClearPlayerSpeedY`, `StopPlayerFalling`, `UpdatePlayer`,
  `PlayerTouchesBox`, `DestroyPlayer`, `GetPlayerCollisionQueue`, `ClearPlayerDead`,
  `SetPlayerDead`, `IsPlayerDead`, `StartPlayerRampX`, `SetPlayerRampX`,
  `sub_800B4F8`, `sub_800B508`, `sub_800B510`, `sub_800B51C`,
  `IsPlayerInvulnerable`, `ClearPlayerInvulnerability`, `SetPlayerInvulnerable`, `SetPlayerControlMode`,
  `GetPlayerControlMode`, `GetPlayerStandingOn`, `SetPlayerStandingOn`, `SetPlayerBusy`,
  `IsPlayerBusy`, `sub_800B584`, `sub_800B58C`, `sub_800B5A0`,
  `sub_800B5A8`, `sub_800B5B0`, `sub_800B5BC`, `sub_800B5C4`,
  `sub_800B5CC`, `sub_800B5D8`, `SetPlayerBumped`, `IsPlayerBumped`,
  `GetPlayerPushRight`, `SetPlayerPushRight`, `GetPlayerPushLeft`, `SetPlayerPushLeft`,
  `IsPlayerHanging`, `SetPlayerHanging`, `IsPlayerSlippery`, `SetPlayerSlippery`,
  `sub_800B650`, `sub_800B678`, `SetCtrlMode`, `SetCtrlAnimSet`,
  `SetCtrlTargetMotionY`, `StartCtrlTargetMotionY` (issues #84/#85 - see
  [docs/matching/issue-84-85-SetCtrlTargetMotionY.md](../matching/issue-84-85-SetCtrlTargetMotionY.md);
  matched with `self`/`vec` pinned to `r3`/`r2` and each branch's X/Y/Z
  locals pinned to their own ABI registers in the ROM's actual load
  order, avoiding the callee-saved spill three earlier attempts hit)

- `src/player/player_init.c` (new file, GitHub issue #9/#10, ROM
  `0x0800B3F0`, non-adjacent to `player_reset.c` since the matched
  `player_update.c`/`ApplyPlayerVelocity`/`player_flags.c` sit between
  them): `InitPlayer` - a part-object constructor re-initializing
  `self` via `InitGroundSprite`, allocating a child `struct actor` via
  `CreateSpriteObj`, hooking it up at `self+0xb0` via the standard
  `ResetSpriteFrameTimer`/`ResetSpriteFrameIndex`/`SetSpriteAnimDone` OAM trio, then calling
  `ResetPlayer` to finish the reset and setting `self`'s `field_08`/
  `x`/`y` from its three `u16` arguments; see
  [docs/matching/issue-9-10-0x0800a884-graphics.md](../matching/issue-9-10-0x0800a884-graphics.md).

- `src/objects/ctrl.c` (new file - see `docs/matching.md`):
  `StartCtrlTargetMotionYFromSet`, `SetCtrlTargetMotionX`, `StartCtrlTargetMotionX`, `StartCtrlTargetMotionXFromSet`,
  `CtrlHandleEvent`, `SetCtrlTargetAnim`, `AttachCtrl`, `DestroyCtrl`,
  `InitCtrl`, `GetCtrlMode`

- `src/player/action_ctrl_states.c`/`action_ctrl_land.c` (new files, non-
  adjacent since `ActionCtrlStateCrawl` sits between them - see
  `docs/matching.md`, issue #17): `ActionCtrlStateStandUp`, `ActionCtrlStateCrawlStart`,
  `ActionCtrlStateCrawl`, `ActionCtrlStateCrawlStandUp`, `ActionCtrlStateBodySlamLand`, `ActionCtrlStateLand` - six
  entries of the `gActionCtrlStateTable` 42-slot action dispatch table
  (`ActionCtrlStateCrawl`/`ActionCtrlStateLand` were NAKED until the second issue
  #15/#16/#17 retry)

- `src/graphics/actor_part19.c`/`actor_part19c.c`/`actor_part19d.c`/
  `actor_part19f.c`/`actor_part19g.c` (new files, non-adjacent since
  the now-matched `CreatePolarCollectedWumpa` (see below), `RunPolarPlayerState`
  (`actor_part19e.c`, now matched too - see below), and one left-raw function sit
  between them - `DrawPolarCollectedWumpa` (`actor_part19b.c`), previously also
  parked here, is now matched as real C (see below) - see
  `docs/matching.md`, issue #52): `PolarPlayerStateLaunched`, `PolarPlayerStateFinish`,
  `PolarPlayerStateLand`, `FinishPolarRun`, `CatchPolarPlayer`, `QueuePolarWumpa`,
  `GivePolarPlayerLife`, `BoostPolarPlayer`, `GivePolarPlayerMask`, `LaunchPolarPlayer`,
  `DestroyPolarPlayer`, `IsPolarPlayerInactive`, `UpdatePolarCollectedWumpa`, `DestroyPolarCollectedWumpa`,
  `UpdatePolarWumpa`, `CreatePolarWumpa`, `UpdatePolarCrate`, `UpdatePolarQuestionCrate`,
  `UpdatePolarLifeCrate`, `UpdatePolarNitroCrate`, `UpdatePolarAkuAkuCrate` - the same large
  per-instance "self" object's action-table/trampoline/circular-list
  conventions as `ctrl.c`/`action_ctrl_states.c`

- `src/graphics/actor_part19i.c` (new file, directly adjacent to
  `actor_part19d.c`'s matched functions - GitHub issue #53):
  `UpdatePolarTimeCrate`, `sub_802CA28`, `sub_802CA6C`, `UpdatePolarBasicCrate` - the
  type-byte-dispatch/proximity "used"-state transition family (same
  shape as `UpdatePolarQuestionCrate`/`UpdatePolarLifeCrate`, `actor_part19g.c`); `InitPolarCrate`
  and its seven thin forwarding wrappers (`CreatePolarTimeCrate`, `CreatePolarQuestionCrate`,
  `CreatePolarAkuAkuCrate`, `CreatePolarNitroCrate`, `CreatePolarLifeCrate`, `sub_802CC54`,
  `CreatePolarBasicCrate`) - an `InitActorPart`-based constructor family
  classifying a "kind" from a `__divsi3`-scaled/clamped value plus a
  range-keyed offset. See
  [docs/matching/issue-53-actor-c7a8.md](../matching/issue-53-actor-c7a8.md).

- `src/graphics/actor_part126.c` (new file, `0x0802CDE4`-`0x0802D2DC`,
  the remainder of the `0x0802CC9C`-`0x0802D3A8` gap between issues #53
  and #54): `CreatePolarElectricFence`/`sub_802CE38`/`CreatePolarLauncher`/`CreatePolarPenguin`/
  `CreatePolarIcicle` - `InitActorPart`-based constructors on the same `self`
  object family; `UpdatePolarLauncher` - a 3-way `self+0x28` state dispatch;
  `UpdatePolarPenguin` plus its `AimPolarPenguin` homing-velocity helper - a
  velocity/proximity state machine; `sub_802CE10`/`UpdatePolarIcicle` - small
  proximity-gated state advances; `RefreshPolarAkuAku`/`UpdatePolarAkuAku` - a
  VRAM-gauge/state-transition pair for a `gPolarAkuAkuInvincibleTimer`-counted
  effect. 12 functions, all matched. See
  [docs/matching/issue-53-issue-54-gap-cc9c.md](../matching/issue-53-issue-54-gap-cc9c.md).

- `src/util/aabb_setup.c` (new file, GitHub issue #70, ROM
  `0x0803AFDC`-`0x0803B060` - right after the parked division/modulo
  trio in `lib/libgcc/lib1funcs.s`, see that file's `docs/matching.md`
  entry): `SetAabbSize`/`SetAabbPos` (the shared AABB set-size/
  set-position primitive already referenced by name from
  `sprite.c`/`power_dialog_draw.c`), `GetLives` (a
  trivial raw-offset getter), `DestroyLargeFont`/`DestroySmallFont` (two more
  `gEntityVtable`-family per-type descriptor table constructors)

- `src/bosses/airship_fireball.c`/`airship_states.c`/`airship_fall.c`/
  `airship.c`/`airship_damage.c`/`airship_graphics.c` (new files, issue
  #58, ROM `0x08030334`-`0x08031784` - the boss-weapon effect state
  machine, non-adjacent since 18 raw functions sit between/around them;
  see
  [docs/matching/issue-58-0x08030334-actor.md](../matching/issue-58-0x08030334-actor.md)):
  `DamageAirshipFireball`, `CreateAirshipFireball`, `AirshipFireballStateExplode`, `IsAirshipFireballUnshootable`,
  `AirshipStateApproach`, `AirshipStateFall`, `UpdateAirshipBg2`, `DamageAirship`,
  `UpdateAirshipFlashColor`, `AnimateAirshipPalette` - a countdown-timer state transition, an
  `InitActorPart`-based constructor, a trivial byte setter/getter pair,
  a camera-relative position accumulator with its own state-2/table-
  index-0 transition, a screen-accumulator/tracker-reset step, a BG2
  zoom-effect updater, a "charge" countdown, and a palette flash/
  animation-refresh pair.
- `src/player/input_ctrl_queue.c` (new file, GitHub issue #22, ROM
  0x08017A44-0x08017AAC - numbered `27` rather than `20` since issue
  #58's parallel PR above independently claimed `actor_part20.c`-
  `actor_part26.c` first): `IsInputCtrlMotionXPending`-`GetCtrlTarget` (9 functions) -
  the same player/action-object family as `action_ctrl_states.c`/
  `actor_part19.c` (`self+0xc` table pointer, `self+0x10` part
  pointer); see `docs/matching/issue-22-0x08017a44-actor.md`.
- `src/bosses/mega_mix.c` (new file, GitHub issue #22, ROM
  0x08017ECC-0x08017FE8, non-adjacent to `input_ctrl_queue.c` since the
  raw `UpdateMegaMix` sits between them): `SetMegaMixMotionYFromSet`, `SetMegaMixMotionXFromSet`,
  `StartMegaMixMotionYFromSet`, `StartMegaMixMotionXFromSet`, `ResetMegaMixCtrl`, `DestroyMegaMixCtrl`,
  `CreateMegaMixCtrl` - a `self+4` double-pointer-chain record lookup (same
  shape as `StartCtrlTargetMotionYFromSet`/`StartCtrlTargetMotionXFromSet`) feeding the
  `gMegaMixMotionRecords` per-vector-component trampoline table; see
  `docs/matching/issue-22-0x08017a44-actor.md`.
- `src/bosses/tiny_hop_pad.c` (new file, GitHub issue #22, ROM
  0x080187FC-0x08018884, non-adjacent to `mega_mix.c` since the
  raw `UpdateTiny`-`SpawnTinyFallingLeaves` block sits between them):
  `UpdateStompedHopPad`, `DestroyStompedHopPadCtrl`, `CreateStompedHopPadCtrl`, `UpdateOneShotAnimCtrl`; see
  `docs/matching/issue-22-0x08017a44-actor.md`.
- `src/bosses/mega_mix_update.c` (GitHub issue #22, ROM
  0x08017AB0-0x08017ECC): `UpdateMegaMix` - the player-vs-part 3-state
  dispatcher, previously a NAKED transcription, now real C built with
  old_agbcc; see `docs/matching/issue-22-0x08018008-hopper.md`.
- `src/bosses/tiny_update.c` (new file, GitHub issue #22, ROM
  0x08018008-0x080187FC, built with old_agbcc): `UpdateTiny`,
  `SetTinyState`, `PickTinyHopTarget`, `SpawnTinyFallingLeaves` - the
  `gTinyVtable` hopping boss's update/enter-state methods,
  target picker and falling-hazard spawner; see
  `docs/matching/issue-22-0x08018008-hopper.md`.
- `src/player/action_ctrl_states.c` (new file, GitHub issue #17, ROM
  0x08013C60-0x08014084, built with old_agbcc): `ActionCtrlStateSpin`,
  `ActionCtrlStateAirSpin`, `ActionCtrlStateTornadoSpin`, `ActionCtrlStateCrouchDown` - four more
  `gActionCtrlStateTable` action-table handlers (`ActionCtrlStateCrouch`, in the
  same file, is parked below); see
  `docs/matching/issue-17-0x08012fbc-actor.md`, "Second pass".
- `src/player/action_ctrl_hang.c` (new file, GitHub issue #17, ROM
  0x08014674-0x08014F8C, built with old_agbcc): `ActionCtrlStateLeftGround` (since
  the mix NAKED retry 5, `docs/matching/mix-naked-retry-5.md`),
  `ActionCtrlStateDying`, `ActionCtrlStateWarpIn`, `ActionCtrlStateHang`, `sub_8014AEC`,
  `ActionCtrlReleaseHang` (since the late NAKED retry 3,
  `docs/matching/late-naked-retry-3.md`), `ActionCtrlStateHangMoveStart`, `ActionCtrlStateHangMove`,
  `ActionCtrlStateHangStop`; see `docs/matching/issue-17-0x08012fbc-actor.md`, "Second
  pass" and "Third pass".
- `src/player/action_ctrl_run_jump.c`, `action_ctrl_states.c` (GitHub issue #17, ROM 0x08012FBC-0x08013C60, now
  built with old_agbcc): `ActionCtrlStateRun`, `ActionCtrlStateJump`, `ActionCtrlStateAirborne`,
  `ActionCtrlStateFlipBodySlamStart`, `ActionCtrlStateSlide` - the chunk's first five action-table
  handlers, formerly NAKED; see
  `docs/matching/issue-17-0x08012fbc-actor.md`, "Third pass".
- `src/player/player_event.c` (GitHub issue #9/#10, ROM
  0x0800AB9C-0x0800AC2C, now built with old_agbcc): `CollidePlayerWithObjects`, the
  big object's teardown/notification step, formerly parked
  NON_MATCHING. The box goes to `CollidePartList` by value; see
  `docs/matching/issue-9-10-0x0800ab9c-graphics.md`.
- `src/bosses/hovercraft_parts.c`/`hovercraft_cannon.c`/`hovercraft_launcher.c` (new files, GitHub issue #62, ROM
  0x08033804-0x08033EF4 - the `gHovercraft` singleton system's
  accessor/state-machine cluster, non-adjacent since the (now matched)
  `HovercraftCannonStateFire`/`HovercraftLauncherStateLaunch` (`hovercraft_cannon.c`/`hovercraft_launcher.c`) and the (now matched)
  `UpdateHovercraftCannon`/`RunHovercraftCannonState`/`UpdateHovercraftLauncher` (see below) sit interleaved
  between them; see
  [docs/matching/issue-62-0x08033804-actor.md](../matching/issue-62-0x08033804-actor.md)):
  `StartHovercraftHitFlash`, `SetHovercraftFlashColor`, `GetHovercraftPartsLeft`, `LoseHovercraftPart`,
  `GetHovercraftAttack`, `GetHovercraftState`, `GetHovercraftLevel`, `GetHovercraftZ`,
  `GetHovercraftY`, `GetHovercraftX`, `SetHovercraftState`, `HovercraftStateInactive`,
  `HovercraftStateApproach`, `nullsub_37`, `DamageHovercraftCannon`,
  `CreateHovercraftCannon`, `HovercraftCannonStateDestroyed`, `HovercraftCannonStateWait`,
  `IsHovercraftCannonUnshootable`, `DamageHovercraftLauncher` - the
  singleton's one-shot latches, field getters, state-transition/
  anim-frame-reset setters, an `InitActorPart`-based constructor, and
  several "self" object accessors/setters sharing the boss cluster's
  layout convention.
- `src/player/action_ctrl_hang.c` (new file `actor_part38.c`, GitHub issue #18, ROM
  0x08014F8C - numbered `38` rather than `28` since issue #62's
  parallel PR above independently claimed `actor_part28.c` first):
  `DoSuperBodySlamShockwave` - a `gCollidableList`-list proximity-
  trigger scan for the same "self" action-table object family as
  `action_ctrl_states.c`, and `StartActionCtrlTornadoSpin` (matched in the strag4 retry,
  see Matched); see
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- `src/player/action_ctrl_moves.c` (new file, GitHub issue #18, ROM
  0x080151C8, non-adjacent to `action_ctrl_hang.c` since `StartActionCtrlTornadoSpin`
  sits between them): `sub_80151C8`, `EndActionCtrlSpin`, `SteerActionCtrlSpin`
  (the last two matched in the strag2 retry); see
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- `src/player/action_ctrl_moves.c` (new file, GitHub issue #18, ROM
  0x08015350-0x080156B4, non-adjacent to `action_ctrl_moves.c` since
  `EndActionCtrlSpin`/`SteerActionCtrlSpin` sit between them):
  `SetActionCtrlMode`, `StartActionCtrlSpin`, `StartActionCtrlHangSpin`, `StartActionCtrlRun`,
  `StartActionCtrlHighJump`, `sub_8015558`, `AttachActionCtrl`, `sub_80155AC`,
  `sub_80155B8`, `ActionCtrlStateHangSpin`, `ActionCtrlStateHangGrab`, `ActionCtrlStateWarpOut`,
  `ActionCtrlStateCrawlStop` - more of the same self+0xc/self+0x10 trampoline-pair
  family, including two near-identical self+0x29-keyed mgr-trampoline
  arms (`StartActionCtrlRun`) and several part+0x38-gated trampoline firers
  and `ActionCtrlStateBodySlamStart` (matched in the strag2 retry); see
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- `src/player/action_ctrl.c` (new file, GitHub issue #18, ROM
  0x0801574C-0x08015840, non-adjacent to `action_ctrl_moves.c` since
  `ActionCtrlStateBodySlamStart` sits between them): `nullsub_17`,
  `ActionCtrlStateTurboRun`, `nullsub_18`, `sub_8015774`, `SetActionCtrlModeAnim`,
  `ActionCtrlSetTargetAnim` - two nullsubs, two tail-call wrappers, the shared
  trampoline-pair-plus-sentinel-store helper called by
  `action_ctrl_states.c`'s `ActionCtrlStateStandUp`/`ActionCtrlStateCrawlStart`, and `ActionCtrlSetTargetAnim`
  (player's `+0x100`-flag-gated `mode` remapper tail-calling
  `SetCtrlTargetAnim`, reinterpreted through a `s32`-returning function-
  pointer cast to steer the epilogue's `pop`/`bx` scratch register
  choice - see
  [docs/matching/naked-sub_80157c4-matched.md](../matching/naked-sub_80157c4-matched.md));
  see `docs/matching/issue-18-0x08014f8c-actor.md`.

- `src/graphics/actor_anim.c` (extended, GitHub issue #71, ROM
  `0x0803B060`-`0x0803B46C` - immediately adjacent to the file's existing
  `GetAnimFrameBaseOffset`, which itself ends exactly at `0x0803B060`):
  `GetAnimFrameAttr` (reads the current keyframe's `attr` halfword pre-shifted
  into the high 16 bits), `GetAnimFrameData` (resolves the current
  keyframe's tile-graphics pointer via `frameOffsets`/`gCategorySpriteSheet`),
  `SetActorAnim` (selects a new keyframe, resetting the playback
  accumulator), `UpdatePolarCheckpointText` (advances a Q8 fall/scroll accumulator,
  then either fires the `self+0x50` trampoline or tail-calls
  `UpdateActor`), and 20 byte-identical `gActorVtable` "kind"
  teardown handlers (`DestroyRiderlessPolar`, `DestroyPolarCheckpointText`, `DestroyPolarWumpa`,
  `DestroyPolarTimeCrate`, `DestroyPolarQuestionCrate`, `DestroyPolarAkuAkuCrate`, `DestroyPolarNitroCrate`,
  `DestroyPolarLifeCrate`, `sub_803B25C`, `DestroyPolarBasicCrate`, `DestroyPolarCrate`,
  `DestroyPolarElectricFence`, `sub_803B30C`, `DestroyPolarLauncher`, `DestroyPolarPenguin`,
  `DestroyPolarIcicle`, `DestroyPolarAkuAku`, `DestroyPolarGoal`, `DestroyPolarBoostPad`,
  `DestroyPolarCheckpointCrate` - unlink `self` from its `+0x48`/`+0x4c` circular list,
  set `+0x50` to the shared "dead" vtable, and conditionally free), and
  `DrawJetpackCheckpointText` (fixed-position OAM setup for one sprite frame) - see
  [docs/matching/issue-71-0x0803b060-actor.md](../matching/issue-71-0x0803b060-actor.md).
- `src/pickups/wumpa.c` (new file, GitHub issue #16, ROM
  0x080119A8-0x08011BD4): `DrawWumpa`, `GetWumpaClassId`, `DestroyWumpa`,
  `ResetWumpaPickup`, `InitWumpa`, `CollideWumpa`, `SetWumpaPos`,
  `SetWumpaHop`, `SetWumpaCounter`, `UpdateStopwatch`, `CreateStopwatch`,
  `ResetStopwatch`, `DestroyStopwatch`, `InitStopwatch`, `ResetActionCtrl` - a run of
  `struct actor` vtable-swap constructor helpers (same
  `InitSpriteObj`/`DestroySpriteObj`/`nullsub` shape as `sprite_obj.c`), a
  handful of small setters/getters on offsets beyond `struct actor`'s
  own 0x1c bytes, and a distance-gate (`UpdateStopwatch`) reusing
  `sprite.c`'s `gEntityFlags+0x108` bitmap idiom verbatim.
  Recategorized `graphics`->`actor` from the issue's label: every
  matched function here operates on `struct actor` via the same
  `table@0x18`/`flags@0xc`/`field_08@8` layout `actor_part*.c` already
  established, not the `game_loop`-core "child object" family
  `docs/rom_map.md` traces through the chunk's remaining (unmatched)
  functions. See
  [docs/matching/issue-16-actor-11b0c.md](../matching/issue-16-actor-11b0c.md).
- `src/player/kill_player.c` (new file, GitHub issue #16, ROM
  0x08012160-0x08012420): `KillPlayer`, `sub_8012238`, `UpdatePlayerFacing` -
  three more members of the 42-slot action-dispatch-table family
  (`gActionCtrlStateTable`), operating on the same still-unnamed "child
  object" struct (`self+0xc`/`self+0x10` sub-record pointers, the
  `+0x27`-`+0x32` state/flag/table-index trio) `action_ctrl_states.c`/
  `action_ctrl_land.c` already established conventions for. Not
  ROM-adjacent to those files (the raw `UpdateActionCtrl`/`TryActionCtrlDoubleJump`/
  `HandleActionCtrlAirInput` and the still-raw `ActionCtrlHandleEvent` sit between them), so
  a new file. See
  [docs/matching/issue-16-actor-12160.md](../matching/issue-16-actor-12160.md).
- `src/player/action_ctrl_left_ground.c` (new file, GitHub issue #16, ROM
  0x08012A7C-0x08012AF4): `CheckActionCtrlLeftGround` - another member of the same
  action-dispatch-table family, not ROM-adjacent to `kill_player.c`'s
  functions either (the raw `UpdateActionCtrl`/`TryActionCtrlDoubleJump`/`HandleActionCtrlAirInput`
  sit in between). See
  [docs/matching/issue-16-actor-12160.md](../matching/issue-16-actor-12160.md).
- `src/graphics/actor_part43.c`/`actor_part44.c`/`actor_part45.c`/
  `actor_part46.c` (new files, GitHub issue #56, ROM
  0x0802F0DC-0x0802FBF0 - a second boss-weapon "spawn/pre-attack"
  singleton and its `self` object, non-adjacent since the parked
  `AllocJetpackPlayerTiles`/`CreateJetpackShot`, `RunJetpackPlayerState`
  (`actor_part44b.c`, now matched - see below), `LoadBgPicture`/
  `FillBgPictureMap` (`actor_part45d.c`: 8E8 matched, 7B0 still NAKED - see
  below) and `UpdateJetpackPlane`
  (`actor_part46b.c`, now matched - see below) sit interleaved between them; see
  [docs/matching/issue-56-0x0802f0dc-actor.md](../matching/issue-56-0x0802f0dc-actor.md)):
  `FinishJetpackRun`, `PassJetpackRing`, `DispenseJetpackWumpa`, `CountJetpackBomber`, `GetJetpackPlayerHpPercent`,
  `SetJetpackCheckpoint`, `IsJetpackPauseLocked`, `AnimateJetpackPlayerPalette`, `HealJetpackPlayer`,
  `QueueJetpackWumpa`, `JetpackPlayerStateResume`, `JetpackPlayerStateBoost`, `JetpackPlayerStateFall`,
  `JetpackPlayerStateFinish`, `JetpackPlayerStateEnter`, `DestroyJetpackPlayer`,
  `IsJetpackPlayerInactive`, `UpdateJetpackShot`, `IsJetpackShotUnshootable` - a constructor/reset, a
  state-machine update, an accumulator-drain/reward-
  dispenser, accessors, accumulator drivers, idle-state-reset idioms,
  and the singleton's teardown/destructor, all sharing
  `ctrl.c`/`action_ctrl_states.c`/`airship_fireball.c`'s established
  "self" object conventions.
- `src/graphics/actor_part_2fbf0.c` (new file, GitHub issue #57 plus
  issue #58's first two functions, ROM 0x0802FBF0-0x08030530, formerly
  `asm/code_3_2_20_28568_c99c_2fbf0.s`): three small C++ actor classes
  (method tables `gJetpackPlaneVtable`/`087E51EC`/`087E5224`) and the
  orbiting-companion updaters - `AimJetpackPlane`, `DamageJetpackPlane`,
  `CreateJetpackPlane`, `JetpackPlaneStateFall`, `sub_802FE1C`, `sub_802FE58`,
  `JetpackPlaneStateFly`, `RunJetpackPlaneState`, `IsJetpackPlaneUnshootable`, `CreateJetpackBomber`,
  `UpdateJetpackBomber`, `HomeJetpackBomber`, `JetpackBomberStateDying`, `JetpackBomberStateDrop`,
  `JetpackBomberStateCircle`, `JetpackBomberStateSwingHorizontal`, `JetpackBomberStateBobVertical`, `JetpackBomberStateHome`,
  `JetpackBomberStateIdle`, `DamageJetpackBomber`, `RunJetpackBomberState`, `IsJetpackBomberUnshootable`,
  `UpdateJetpackCannonball`, `CreateJetpackCannonball`, `IsJetpackCannonballUnshootable`, `AirshipFireballStateOrbit`,
  `AirshipFireballStateSpiralIn` (all 27 real C, including the pointer-to-member
  dispatch shape parked NAKED elsewhere as the "r7 hazard"). First user
  of the shared `include/actor_self.h`. See
  [docs/matching/issue-57-0x0802fbf0-actor.md](../matching/issue-57-0x0802fbf0-actor.md).
- `src/graphics/actor_part50.c`/`actor_part51.c`/`actor_part52.c`/
  `actor_part53.c`/`actor_part54.c`/`actor_part55.c`/`actor_part56.c`
  (new files, GitHub issue #50, ROM 0x0802A69C-0x0802AC28 - numbered
  `50`-`56` rather than `39`-`45` since issues #16 and #56's parallel
  PRs above independently claimed those numbers first; see
  [docs/matching/issue-50-actor-2a69c.md](../matching/issue-50-actor-2a69c.md)):
  `JetpackReloadPlayerTiles`, `PolarReloadPlayerTiles`, `JetpackReachCourseEnd`, `PolarReachCourseEnd`,
  `IsTouchingPlayer`, `InitActorPart`, `UpdateActor`, `UpdateActorDepth`,
  `GetActorRecordIndex`, `SetActorState`, `GetActorZ`, `GetActorY`,
  `GetActorX`, `IsActorVisible`, `DestroyActor`, `IsSpawnCollected`,
  `MarkSpawnCollected`, `ClearCollectedSpawns`, `RestoreActorPaletteCycle`, `SaveActorPaletteCycle`,
  `SetActorPaletteCycle`, `EnableActorPaletteCycle` - the `InitActorPart` constructor itself
  (previously only forward-declared by every other `actor_part*.c`
  file), its movement-threshold recompute pair, the fixed 15-slot
  object registry (`gCollectedSpawns`/`gCollectedSpawnCount`), and the
  `gActorPaletteCycleEnabled`-gated palette-cycle DMA cluster's members - plus
  `DrawActor`, `sub_802AA0C`, and `UpdateActorPaletteCycle`, all three
  matched in a later pass that closed the register-pinning/pool-split
  gaps documented in that same writeup (all 25 of this chunk's functions
  are now real C, none NAKED).
- `src/graphics/actor_part127.c` (new file, ROM `0x0802B364`-`0x0802BC68`
  - the start of the actor zone before issue #52, right before
  `actor_part107.c`'s own range): `PolarPlayerStateShocked` - a frame-counter
  threshold DMA driver sharing the same reset idiom as `HurtPolarPlayer` -
  plus `HurtPolarPlayer`, `ShockPolarPlayer`, and `PolarPlayerStateRun` themselves,
  promoted from NAKED using the `goto`-shared-tail idiom (an explicit
  `goto`/single shared `return` reproducing the ROM's own branch layout
  where an early-return case shares its epilogue with the main
  fallthrough path, instead of a plain `if`/`return` guard clause this
  compiler would inline differently) - see
  [docs/matching/issue-52-gap-b364.md](../matching/issue-52-gap-b364.md).
  All 11 of this gap's functions are now real C: the other 7
  (`UpdatePolarPlayer`, `DrawPolarPlayer`, `AllocPolarPlayerTiles`, `PolarPlayerStateMount`,
  `PolarPlayerStateJump`, `PolarPlayerStateDash`, `PolarPlayerStateCaught`) were promoted from NAKED
  in the issue #51/#54 retry, with the file switched to old_agbcc - see
  [docs/matching/issue-51-54-naked-retry.md](../matching/issue-51-54-naked-retry.md).
- `src/graphics/actor_part107.c` (new file, ROM 0x0802BC68-0x0802BED8 -
  the literal tail of `asm/code_3_2_20_8b7c_ac28.s`, one raw file's
  leftover portion out of GitHub issue #50's original chunk scope;
  everything before it in that raw file - `CreateActor`'s giant
  kind-dispatch actor-part-factory constructor and the run of actor-
  part-factory/animation-table-state functions between it and here -
  stayed raw then, since matched in `actor_part_2ac28.c` (below); see
  [docs/matching/issue-50-actor-bc68.md](../matching/issue-50-actor-bc68.md)):
  `DispensePolarWumpa`, `IsPolarPauseLocked`, `sub_802BD24`, `PolarPlayerStateFinishLeap`,
  `PolarPlayerStateCarriedOff`, `PolarPlayerStateKnockedOff`, `PolarPlayerStateBoost` - an accumulator-drain/
  reward-dispenser (docs/rom_map.md already reads it as a structural
  twin of `actor_part44.c`'s `DispenseJetpackWumpa`), a trivial byte getter, two
  frame-counter-threshold state-reset functions sharing the state/
  table-index/anim-frame reset idiom, and a three-axis hazard-threshold
  driver family (screen-flash trigger via `FadeBrightness`, hazard-
  direction arming via `SetActorCategoryExitStatus`) on the same `gUnknown_0300148x`/
  `gUnknown_030014Ax` global cluster `actor_part19.c`/`actor_part44.c`
  already established; matched.

- `src/player/action_ctrl.c` (new file, GitHub issue #19, ROM
  0x08015840-0x080159A4 - recategorized `graphics`->`actor` from the
  issue's label, same self+0xc/self+0x10 trampoline-pair and state/
  counter/table-index-trio family as action_ctrl_moves.c/action_ctrl.c;
  immediately adjacent to action_ctrl.c's matched span): `RestartActionCtrl`, `DestroyActionCtrl`,
  `InitActionCtrl`, `sub_80158AC`, `SetActionCtrlMotionYKeepSpeed`, `SetActionCtrlMotionXKeepSpeed`,
  `SetActionCtrlMotionYPending`, `SetActionCtrlMotionXPending`, `ClearActionCtrlMotionYPending`, `ClearActionCtrlMotionXPending`,
  `IsActionCtrlMotionYPending`, `IsActionCtrlMotionXPending`, `QueueActionCtrlMotionYKeepSpeed`, `QueueActionCtrlMotionXKeepSpeed`,
  `QueueActionCtrlMotionY`, `QueueActionCtrlMotionX`, `sub_8015950`, `ResetPlayerCtrl`,
  `RestartPlayerCtrl` - a run of small accessors/resetters on the state-trio
  bytes, the `gActionCtrlVtable` double-table-set idiom already seen
  in `input_ctrl_queue.c`, and a larger field-reset pair; see
  [docs/matching/issue-19-0x08015840-actor.md](../matching/issue-19-0x08015840-actor.md).
- `src/player/swim_ctrl_stroke.c` (GitHub issue #19, ROM
  0x080159F8-0x08015FDC, built with old_agbcc): `StartPlayerCtrlStroke`,
  `StartPlayerCtrlSpin`, `ApplyPlayerCtrlSwimDrift` - the player-input controller's three
  jump-table dispatchers (`level`-indexed speed tables, a kind-4 spawn and
  the `SetPlayerSwimDriftX`/`SetPlayerSwimDriftY` feed), promoted from NAKED once built
  with old_agbcc; see
  [docs/matching/issue-19-0x08015840-actor.md](../matching/issue-19-0x08015840-actor.md).
- `src/player/swim_ctrl_drift.c` (new file, GitHub issue #19, ROM
  0x08015FDC, non-adjacent to action_ctrl.c since
  `StartPlayerCtrlStroke`/`StartPlayerCtrlSpin`/`ApplyPlayerCtrlSwimDrift` (`swim_ctrl_stroke.c`)
  sit between them):
  `SetPlayerSwimDriftY` - a player-velocity-relative record writer; see
  [docs/matching/issue-19-0x08015840-actor.md](../matching/issue-19-0x08015840-actor.md).
- `src/graphics/actor_part58.c` (new file, GitHub issue #54, non-
  adjacent to `actor_part56.c` since the whole 0x0802D3A8-0x0802E0A4
  range sits between them; numbered `58` rather than `57` since issue
  #19's PR independently claimed `actor_part57.c`/`57b.c` first - see
  [docs/matching/issue-54-actor-d3a8.md](../matching/issue-54-actor-d3a8.md)):
  `ClearPolarAkuAkuMask`, `RemovePolarAkuAkuMask`, `AddPolarAkuAkuMask`, `CreatePolarAkuAku`,
  `SetPolarMaskLevel`, `GetPolarMaskLevel`, `UpdatePolarGoal`, `CreatePolarGoal`,
  `UpdatePolarBoostPad`, `CreatePolarBoostPad`, `UpdatePolarCheckpointCrate`, `CreatePolarCheckpointCrate` -
  `InitActorPart`-based constructor variants plus the
  `gLevelState+0x78` Aku-Aku-mask-style add/remove pair.
- `src/graphics/actor_part59.c` (new file, GitHub issue #54, non-
  adjacent since `actor_part74.c` sits between it and `actor_part58.c`;
  see
  [docs/matching/issue-54-actor-d3a8.md](../matching/issue-54-actor-d3a8.md)):
  `YetiStateChase`, `YetiStateCharge` - the `gYeti` position-
  tracking object's two `gYetiStateFuncs` vtable-slot update
  functions (accumulate/clamp, tier-keyed `PlaySfx`/`PlayAmbientSfx`
  cues, and a shared kind/anim-reset transition tail).
- `src/graphics/actor_part60.c` (new file, GitHub issue #54, non-
  adjacent since `actor_part75.c` sits between it and `actor_part59.c`;
  see
  [docs/matching/issue-54-actor-d3a8.md](../matching/issue-54-actor-d3a8.md)):
  `StopYeti`, `DestroyYeti`, `CreateYeti` - the
  `gYeti` object's state-flag setter, destructor, and
  constructor.
- `src/graphics/actor_part61.c` (new file, GitHub issue #54, non-
  adjacent since `actor_part76.c` sits between it and `actor_part60.c`;
  see
  [docs/matching/issue-54-actor-d3a8.md](../matching/issue-54-actor-d3a8.md)):
  `YetiStateCaught` - a genuine no-op stub.
- `src/graphics/actor_part62.c`, `actor_part74.c`, `actor_part76.c`
  (GitHub issue #54, promoted from NAKED in the issue #51/#54 retry -
  see
  [docs/matching/issue-51-54-naked-retry.md](../matching/issue-51-54-naked-retry.md)):
  `MovePolarAkuAku` (per-state position easing), `UpdateYetiPalette`/`UpdateYetiBg2`
  (the `gYeti` gauge's palette ramp and affine BG2 setup;
  `actor_part74.c` now builds with old_agbcc) and `sub_802E058` (an
  unused copy of the gauge's dot-pattern fill).
- `src/graphics/actor_part128.c` (new file, ROM `0x0802E0A4`-
  `0x0802F0DC`, the gap between issue #54's chunk and issue #56's
  chunk, tracked as issue #55; built with old_agbcc): all 25 functions -
  the spawn dispatcher `CreateJetpackActor` (a plain 31-case `switch`) with
  `SpawnJetpackActor` and its kind constructors, and the player vehicle
  object (`CreateJetpackPlayer`/`InitJetpackPlayer` constructor, `UpdateJetpackPlayer`
  update, `DrawJetpackPlayer` sprite draw, `DamageJetpackPlayer` damage,
  `SteerJetpackPlayerY`/`SteerJetpackPlayerX` steering, `JetpackPlayerStateFly`-`JetpackPlayerStateRollRight`
  state steps). 4 matched in the first pass, the other 21 promoted
  from NAKED in the retry pass. See
  [docs/matching/issue-54-issue-56-gap-e0a4.md](../matching/issue-54-issue-56-gap-e0a4.md)
  and [docs/matching/issue-55-naked-retry.md](../matching/issue-55-naked-retry.md).
- `src/bosses/hovercraft_launcher.c`/`hovercraft_side_gun.c`/
  `hovercraft_cannon_flash.c`/`starfield.c`
  (new files, GitHub issue #63, ROM 0x08033EF4-0x08034AA4 - three
  `InitActorPart`-rooted "self" object kinds immediately following
  issue #62's cluster, non-adjacent since 2 remain parked
  (`UpdateHovercraftCannonFlash`/`sub_8034314`, in `hovercraft_cannon_flash.c` -
  `CreateHovercraftSideGun`/`InitStarfield`/`DrawStarfield`/`SpawnStar`/`PlotStarfieldPixel`
  are now matched too, closing `starfield.c` entirely, see below),
  `RunHovercraftLauncherState`
  (`hovercraft_launcher.c`, now matched - see below), and 3 left-raw functions sit
  interleaved between them; numbered `63`-`73` rather than `57`-`67`
  since issues #19 and #54's PRs independently claimed
  `actor_part57.c`-`62.c` first - see
  [docs/matching/issue-63-0x08033ef4-actor.md](../matching/issue-63-0x08033ef4-actor.md)):
  `CreateHovercraftLauncher`, `HovercraftLauncherStateDestroyed`, `HovercraftLauncherStateWait`, `IsHovercraftLauncherUnshootable`,
  `DamageHovercraftSideGun`, `UpdateHovercraftSideGun`, `sub_80341F8`, `IsHovercraftSideGunUnshootable`,
  `DamageHovercraftCannonFlash`, `CreateHovercraftCannonFlash`, `IsHovercraftCannonFlashUnshootable`, `SpawnStar`,
  `PlotStarfieldPixel`, `UpdateStarfield`, `StarfieldWaitForButton`, `DestroyStarfield` - two
  constructors, a trampoline-fire helper, a position-sync/state-transition
  helper, a damage/death handler, a position-sync/orbit-effect updater and
  its non-identical near-twin, two trivial getters, a no-op stub, a
  128-slot particle-spawner (needed a swapped multiply operand order to
  match this compiler's own left-operand-materializes-into-dest choice; no
  register pins or opaque asm required), a 4-bit tilemap nibble writer
  (needed a `u32`-typed intermediate mask to avoid a spurious 16-bit
  truncation sequence, split address-half statements to pin evaluation
  order, and one opaque `asm volatile` for the ROM's own redundant
  compute-then-copy tail - see that file's doc comments for the full
  account), a particle-spawn-budget driver, an input-poll busy-wait, and a
  buffer-release/teardown helper.
- **`InitContinuePrompt`** (`src/menus/continue_prompt_init.c`, GitHub issue #63) - a
  standalone `struct continue_prompt` object's constructor half: allocates
  and loads its three BG scratch buffers, builds DISPCNT, hands off to
  `InitContinuePromptGraphics`, then builds the BLDCNT/BLDALPHA alpha-blend value.
  Previously parked (`NON_MATCHING`) over a final handful of accumulator/
  temp-register choices in the BLDCNT/BLDALPHA byte-packing tail; now
  matched as real C - both gaps were ordinary register-pin/barrier fixes,
  not a genuine compiler limitation (a `register u8 asm("r1")` pin for one
  stray reload, and an empty `asm("":"+r"(tmp))` compiler barrier to stop
  this compiler from eliding a mask-to-accumulator copy the ROM's own
  build keeps) - see
  [docs/matching/issue-63-final-raw-actor.md](../matching/issue-63-final-raw-actor.md).

- `src/graphics/actor_anim.c` (extended, GitHub issue #72, ROM
  0x0803B4EC-0x0803B8B0 - directly contiguous with this file's existing
  coverage, which already ended right at 0x0803B4EC): `UpdateJetpackCheckpointText` (an
  animation-frame-advance/loop-back function, plus a `+0x50` trampoline
  dispatch when the "held" flag is set) and 15 more "kind" teardown/
  dispatch handlers through `DestroyHovercraftCannonFlash` (the same `struct linked_node`
  unlink-and-free shape, and the same `DestroyJetpackBalloonCrate`-based teardown shape,
  already established earlier in this file). Also recovered 7 functions
  the original disassembly never gave their own `thumb_func_start` label
  for, sandwiched inside what looked like padding/literal-pool gaps
  between the labelled ones (`IsJetpackCheckpointTextUnshootable`, `DestroyJetpackCheckpointText`, `UpdateJetpackExplosion`,
  `IsJetpackExplosionUnshootable`, `GetActorHp`, `DamageActor`, `IsJetpackPlayerUnshootable` - see
  `expected/corrections.txt`'s matching `split` entries, and
  `docs/matching/issue-72-0x0803b4ec-actor.md` for how each was found
  and confirmed via a from-scratch `arm-none-eabi-as`+`objdump`
  reassembly of the original raw block, not by eyeballing the
  disassembly's padding). `struct anim_frame_record`'s `unknown_04[4]`
  became two named `s16` fields (`loopThreshold`/`loopBase`), both read
  by `UpdateJetpackCheckpointText`.

- `src/menus/continue_prompt_init.c`/`actor_part100.c`/`continue_prompt.c`/
  `actor_part95.c`/`continue_prompt.c`/`actor_part96.c`/`actor_part90.c`/
  `actor_part97.c`/`actor_part91.c`/`actor_part98.c`/`actor_part92.c`
  (new files, GitHub issue #48, ROM 0x080291A4-0x08029E4C):
  `SetupActorVramPool` (pins the category's tile-cache slots and
  rebuilds its status-icon OAM row), `CountCategoryCrates` (counts
  `sub_effect_table` entries matching a type-dependent "kind" byte
  set), and the rest of a BG-tilemap double-buffer scroll-effect
  subsystem interleaved in this same ROM region (`AddActorMissedNitro`-
  `sub_8029E40`, minus the NAKED functions below) - see
  [docs/matching/issue-48-0x080291a4-actor.md](../matching/issue-48-0x080291a4-actor.md).
- `src/graphics/actor_part92.c`/`actor_part99.c`/`actor_part93.c`/
  `actor_part94.c` (GitHub issue #49, ROM 0x08029E4C-0x0802A69C):
  `nullsub_6`, `CommitActorBgScroll`, `GetActorBgCenterY`, `GetActorBgCenterX` (the
  BG2-affine scroll subsystem's tail), the `gActorSpawnTable`
  `sub_effect_table` record accessor family (`GetActorCategoryFrameCount`-
  `UpdateActorCategoryBg2`/`SetActorCategoryExitStatus`), and a circular-list marker-drawing pass
  (`DestroyAllActors`) - see
  [docs/matching/issue-49-0x08029e4c-actor.md](../matching/issue-49-0x08029e4c-actor.md).

- **`ResolvePlayerContact`** (`src/objects/player_contact.c`) - fires a
  `part->table+0x68`-driven trampoline based on `gLevelState`'s
  mode, on the player and/or `part` depending on the mode value. A
  `switch` reproduces the ROM's exact 3-way mode dispatch, and explicit
  `goto`s into a shared, ABI-register-pinned tail reproduce the
  mode-0/mode-1-2 call sharing. The remaining conditional-branch
  encoding gap in the mode-3 case (ROM's 3-instruction `cmp;beq;b`
  versus this compiler's usual 2-instruction `cmp;bne` collapse) closed
  once the mode-3 `checkMode3:`/`if (mode == 3)` block was moved to be
  the *last* thing in the function (after `mode0`/`mode1or2`/`tail`
  instead of right after the `switch`) - with the call body no longer
  textually adjacent to its own dispatch test, this compiler's
  block-layout pass can't collapse the two paths and emits the real
  3-instruction form on its own, no inline asm needed. Also needed
  `mode0`'s source order swapped ahead of `mode1or2` (matching ROM's
  real address order - the compiler places case bodies in source order,
  not case-value order) and an explicit `r0` register pin on `mode0`'s
  `player` local (this compiler otherwise picks `r2` there, since
  `mode1or2`'s own `_call_via_r4` call already forces the same value
  into `r0` via the ABI, but `mode0` has no such call to hint it).
  Retires the old raw `asm/code_3_2_15.o` guard entirely.

- `src/graphics/actor_part125.c` (new file, ROM 0x08031784-0x08031A6C,
  Phase 1 of the boss-weapon/singleton cluster's gap between issue #58
  and issue #62): `GetAirshipHpPercent` (tracker "ready" check scaling the
  countdown via `__divsi3`), `DestroyAirship` (tracker destructor,
  `CreateAirship`'s counterpart), `nullsub_30`/`AirshipStateInactive`/`nullsub_32`
  (no-op stubs), `ClearJetpackBalloonCrate` (trivial `self+0x58` setter),
  `ReleaseJetpackBalloon` (full state/accumulator/anim-frame reset idiom),
  `CreateJetpackBalloon` (`InitActorPart`-based constructor), and `IsJetpackBalloonUnshootable`
  (trivial `self+0x5c` getter, needed a trailing `asm(".align 2, 0")`
  for the same lone-function-at-end-of-TU padding gap as
  `IsAirshipFireballUnshootable`/`IsHovercraftCannonUnshootable`) - see
  [docs/matching/issue-59-0x08031784-actor.md](../matching/issue-59-0x08031784-actor.md).

- `src/graphics/actor_part129.c` (new file, ROM 0x08031B0C-0x08032688,
  first 30 of issue #59 Phase 2's 60-function remainder): the
  "type-byte event dispatch" family (`UpdateJetpackQuestionCrate`/`DamageJetpackQuestionCrate`/
  `UpdateJetpackHealthCrate`/`UpdateJetpackTimeCrate`/`DamageJetpackTimeCrate`), three `SpawnJetpackBalloon`-based
  "spawn effect type N" constructors (`CreateJetpackTimeCrate`/`CreateJetpackHealthCrate`/
  `CreateJetpackQuestionCrate`), trivial setters/getters (`ClearJetpackCrateBalloon`/`IsJetpackParachuteNitroUnshootable`/
  `IsJetpackBalloonCrateUnshootable`/`IsJetpackRocketUnshootable`), a full reset idiom (`BreakJetpackBalloonCrate`), a
  countdown-gated trampoline-flush transition (`DamageJetpackBalloonCrate`), a
  doubly-linked-list unlink/`mem_free` destructor (`DestroyJetpackBalloonCrate`), a
  no-op stub (`nullsub_33`), a trivial accumulator (`JetpackBalloonCrateStateFall`), a
  second independent orbital-motion consumer of the shared trig table
  `gSineTable` (`JetpackBalloonCrateStateHang`, alongside the already-flagged
  `UpdateJetpackRocket`), a state-1 trampoline-flush/proximity transition
  (`UpdateJetpackParachuteNitro`), more countdown transitions (`DamageJetpackParachuteNitro`/
  `DamageJetpackRocket`), `UpdateJetpackRocket` itself (the orbital-motion consumer,
  plus its state-transition helper `LaunchJetpackRocket`), a type-byte-gated
  proximity check (`UpdateJetpackRing`), and (promoted from NAKED in a later
  pass) **`CreateJetpackParachuteNitro`**, another `InitActorPart`-based constructor
  forcing a fixed `0xFFFF0600` bias - see
  [docs/matching/issue-59-60-gap-31a6c-part1.md](../matching/issue-59-60-gap-31a6c-part1.md)
  and
  [docs/matching/issue-59-60-m-operand-scheduling.md](../matching/issue-59-60-m-operand-scheduling.md).
  **`InitJetpackBalloonCrate`** (the parameterized `SpawnJetpackBalloon`-based constructor)
  and **`CreateJetpackRocket`** (the clamping `InitActorPart`-based constructor)
  were promoted from NAKED in a still later pass - the first as plain C,
  the second by passing its `0xfa00` constant through a static inline
  wrapper around `InitActorPart` - see
  [docs/matching/issue-59-60-m-operand-scheduling.md](../matching/issue-59-60-m-operand-scheduling.md).
- `src/bosses/hovercraft.c` (new file, ROM 0x080326E4-0x08033804,
  Phase 2 second half of the boss-weapon/singleton cluster's gap between
  issue #58 and issue #62 - a sibling pass, `actor_part129.c`, covers
  the first half, `0x08031A6C`-`0x080326E4`): `CreateJetpackRing` (`InitActorPart`
  constructor), `IsJetpackRingUnshootable`/`IsJetpackCollectedWumpaUnshootable` (trivial "true" getters),
  `CreateJetpackCollectedWumpa` (homing "spawn effect" constructor, byte-for-byte twin
  of `CreatePolarCollectedWumpa`), `DamageHovercraftFireball`/`HovercraftFireballStateFly` (damage/death-transition
  and patrol-speed decay + death transition, sharing the same
  state/anim-frame reset tail as `ReleaseJetpackBalloon`), `CreateHovercraftFireball`
  (`InitActorPart` constructor, same shape as `CreateHovercraftCannon`, `c`
  pinned to `r8`), `HovercraftFireballStateExplode`/`IsHovercraftFireballUnshootable` (trivial `self+0x68`
  byte setter/getter), `UpdateHovercraftHitFlash` (the P2 VRAM-meter's palette
  blink/flash effect, gated by `StartHovercraftHitFlash`'s one-shot latch),
  `HovercraftStateFall` (singleton patrol/oscillation driver, structurally
  parallel to the boss cluster's `AirshipStateFireballs`), `UpdateHovercraftBg2`
  (singleton's own BG2 affine-matrix committer, mirrors `UpdateAirshipBg2`),
  `DestroyHovercraft`/`nullsub_34`/`sub_80337FC`/`nullsub_35` (singleton
  destructor, no-op stub, trivial "false" getter, no-op stub) - opens
  the singleton's own camera-follow/scroll-velocity RAM family
  (`gHovercraftMapCols`-`030015FF`, reusing `hovercraft_parts.c`'s existing
  naming for the fields that family already touches) - see
  [docs/matching/issue-60-61-gap-31a6c-part2.md](../matching/issue-60-61-gap-31a6c-part2.md).
  `RunHovercraftState` (the P1/P2 speed-toggle dispatcher + category-vtable
  animation dispatch) was originally NAKED-parked here too, then
  promoted to real C in a follow-up pass via the static-inline anti-CSE
  technique - see
  [issue-59-60-static-inline-cse-promotion.md](../matching/issue-59-60-static-inline-cse-promotion.md).
- `src/frontend/credits.c` (new file, ROM 0x08034AA4-0x080354E0,
  GitHub issue #64): `GetContinuePromptBlink` (the fade overlay's Yes/No-dialog
  blink/toggle helper), `CommitContinuePromptFrame` (fade overlay per-frame "yield"
  helper), `DestroyContinuePrompt` (fade overlay teardown), `RunContinuePrompt` (the
  "Are you sure?" confirmation-dialog trigger), `CreditsLoop` (the
  between-level map/progress screen's per-frame driver - needed a
  register-pinned `mask &= p->pressed` accumulate-into-existing-register
  rewrite of a plain `&` boolean test, plus a `void *unused` parameter
  kept on `CommitCreditsFrame` since the ROM's own call site passes `self` into
  it even though the callee never reads it), `CommitCreditsFrame` (map screen
  end-of-frame commit), `DestroyCredits` (map screen teardown), and
  `RunCredits` (the map screen's top-level entry point) all matched as
  real C; `DrawContinuePrompt`, `InitCredits`, `DrawCreditsText` and `UpdateCreditsText`
  (first transcribed as NAKED) were promoted to real C in the issue
  #64/#65 NAKED retry, which also moved the object to old_agbcc (the
  whole file matches under it; `InitCredits`, `DrawCreditsText` and
  `UpdateCreditsText` need it) - see
  [docs/matching/issue-64-65-naked-retry.md](../matching/issue-64-65-naked-retry.md);
  `LoadCreditsLogos` was the last NAKED function there and is real C since
  the size2 NAKED retry
  ([docs/matching/size2-naked-retry.md](../matching/size2-naked-retry.md)) - see
  [docs/matching/issue-64-0x08034aa4-actor.md](../matching/issue-64-0x08034aa4-actor.md).

- `src/graphics/actor_part19e.c` (`RunPolarPlayerState`),
  `airship_fireball.c` (`UpdateAirshipFireball`), `airship_fireball.c` (`RunAirshipFireballState`),
  `hovercraft_cannon.c` (`UpdateHovercraftCannon`), `hovercraft_cannon.c` (`RunHovercraftCannonState`),
  `hovercraft_launcher.c` (`UpdateHovercraftLauncher`), `actor_part44b.c` (`RunJetpackPlayerState`),
  `actor_part46b.c` (`UpdateJetpackPlane`), `hovercraft_launcher.c` (`RunHovercraftLauncherState`),
  all of `actor_part125.c` (`UpdateJetpackBalloon`, `DamageJetpackBalloon`,
  `MoveJetpackBalloon`, `JetpackBalloonStatePop`, `JetpackBalloonStateFloatAway`, `RunJetpackBalloonState`),
  `actor_part129.c`'s `UpdateJetpackBalloonCrate`/`RunJetpackBalloonCrateState`, and
  `hovercraft.c`'s `UpdateJetpackCollectedWumpa`/`DestroyJetpackCollectedWumpa`/`UpdateHovercraftFireball`/
  `RunHovercraftFireballState` - formerly NAKED as the "r7 table-base-pin hazard".
  That shape is gcc 2.x's pointer-to-member-function call
  `(this->*table[this->state])()`, reproduced by `ACTOR_PMF_CALL`
  (`include/actor_self.h`) with no register pins. The same pass closed
  the anim-frame-advance idiom's "`#4`/`#6` scheduling gap" by
  re-indexing `anims[animIndex]` per field, and the destructor
  `DestroyJetpackCollectedWumpa` compiled to the ROM's parameter-copy order in plain C.
  See [docs/matching/pmf-dispatch-retry.md](../matching/pmf-dispatch-retry.md).
- `src/graphics/actor_part_2ac28.c` (new file, GitHub issue #51, ROM
  `0x0802AC28`-`0x0802B364`, formerly `asm/code_3_2_20_8b7c_ac28.s`, now
  retired): `CreateActor`, `CreatePolarCheckpointText`, `SpawnPolarCollectedWumpa`, `SpawnPolarAkuAku`,
  `ConstructAnimTableState`, `SpawnActor`, `ConstructActorPart` - the
  per-kind actor factory (a 39-case switch of inlined `new Foo(...)`
  constructors over the `struct anim_table_record` table at
  `gActorAnimTable`), three fixed-record constructors, category vtable
  slots 0/1 (install the animation table and build the player; turn a
  level spawn record into a factory call) and the player constructor.
  All 7 real C, current agbcc (both compilers match). See
  [docs/matching/issue-51-actor-2ac28.md](../matching/issue-51-actor-2ac28.md).
- **Second near-miss sweep:** `InitCellAnim` (`actor_part95.c`, console
  geometry setup) and `SelectActorCategory` (`actor_part102.c`) promoted
  from NAKED to real C, both matching under either compiler. See
  [near-miss-polish-2.md](../matching/near-miss-polish-2.md).

### Matched in the strag4 retry (issue #18's last function)

- `src/player/action_ctrl_hang.c` - `StartActionCtrlTornadoSpin` (issue #18, formerly
  NAKED), old_agbcc. The `self+0x24 != 0` arm tests `self[0x22]`
  directly instead of through a `u8` local, so the byte load lands
  after old_agbcc GCSE's end-of-block copy of `self + 0x22`, and one
  no-code `r1` hold spans the test. The strag2 draft's `r0`/`r2` holds
  are gone.

See [docs/matching/strag4-naked-retry.md](../matching/strag4-naked-retry.md).

### Matched in the strag2 retry (issue #62 raw pair, issue #18 NAKED)

- `src/bosses/hovercraft_cannon.c` - `HovercraftCannonStateFire`, and
  `src/bosses/hovercraft_launcher.c` - `HovercraftLauncherStateLaunch` (issue #62's proximity
  detector pair, formerly raw asm behind `.if NON_MATCHING == 0`; the
  `.s` files and their `ldscript.txt` lines are gone). No pins: both
  divisions are plain `/` through the ROM's `__divsi3`, the new cooldown
  is stored at one shared label, and `u8` zero locals via `"=r"`/`"0"`
  escapes. Both compilers produce the same code; the objects stay on agbcc.
- `src/player/action_ctrl_moves.c` - `EndActionCtrlSpin`, `SteerActionCtrlSpin`, and
  `src/player/action_ctrl_moves.c` - `ActionCtrlStateBodySlamStart` (issue #18, formerly
  NAKED). The fixes were real `u8 *`/`u8` parameters, `u8` locals for the
  table indices, and a constant-copy escape for `EndActionCtrlSpin`'s `0x200`
  test. `action_ctrl_hang.o`/`38b.o`/`38c.o` joined `OLD_AGBCC_OBJS`: their
  whole `.text` is identical under both compilers, and the range is
  confirmed old_agbcc territory.
- Still NAKED at the time: `StartActionCtrlTornadoSpin` (`action_ctrl_hang.c`), with a C
  draft that was 2 halfwords off (matched in the strag4 retry).

See [docs/matching/strag2-naked-retry.md](../matching/strag2-naked-retry.md).

### Matched in the DrawAffineSpritePieces retry

- `src/gfx/affine_sprite_pieces.c` - `DrawAffineSpritePieces` (the affine sibling
  of `DrawSpritePieces`: one affine OAM entry per visible piece, pulled
  towards the first piece's centre by the scale), old_agbcc
  (`affine_sprite_pieces.o` joined `OLD_AGBCC_OBJS`; it is the only function in
  the file). Was 468 halfwords off. Taking the size-table addresses
  before reading `pos` makes reload spill r7, which sets the ROM's
  reload-register rotation. The rest: the pull maths interleaved per
  axis, `k = idx * 4` first, shape/size through inline helpers so the
  `& 3` masks survive, and one extra reference on the matrix param.
  See [docs/matching/graphics-7634-retry.md](../matching/graphics-7634-retry.md).

### Matched in the last-four NAKED retry

- `src/player/player_collide.c` - `CollidePlayer` (the per-frame
  "kind" dispatcher with the camera-probe tail), old_agbcc
  (`player_collide.o` joined `OLD_AGBCC_OBJS`; it is the only function in
  the file). The ROM's walking flag offsets (`adds r1, #3`,
  `subs r2, #3`) are reload's move2add on a reused reload register.
  r3 holds (no code) over the `self+0x70` method lookup, the case-1
  `self+0x68` lookup, and from the busy-flag clear to the end of the
  kind switch keep reload rotating through r0-r2. The kind-5/7/10
  `= 1` stores use the constant-init asm so the 1 is set before the
  address, the case-1 `unk_8c = 0` stores an r0 zero through a pointer,
  and the offset-table result is an r3 register variable. See
  [docs/matching/last-four-naked-retry.md](../matching/last-four-naked-retry.md).

### Matched in the inline-argument-order retry

- `src/player/player_event.c` - `DrawPlayer` (the dizzy-stars
  orbit update), old_agbcc (the object was already on
  `OLD_AGBCC_OBJS`). The last 40 halfwords were the orbit tail. It now
  passes its two sums straight to an inline setter
  (`SetChildPos(child, hist.x + tbl[t & 0xff] * 16, hist.y + tbl[(t >> 1) & 0xff] * 8 - 0x1800)`).
  gcc 2.x expands all of an inline call's arguments, leaving each sum
  unforced, before it copies them into the parameters. That gives the
  ROM's order: history addresses and table reads first, then the
  `child` load, the shifts, the history loads and the adds. The r4/r5
  swap went away with it. See
  [docs/matching/inline-arg-order-retry.md](../matching/inline-arg-order-retry.md).

### Matched in the issue #9 hold pass

- `src/crates/crate_list_update.c` - `UpdateCrateList` (the 3-bucket grid
  maintenance pass), old_agbcc (`crate_list_update.o` joined
  `OLD_AGBCC_OBJS`; it is the only function in the file). A
  hard-register hold on r2, only in the first loop's inlined search
  and only from the found-test to the `base[i]` read, pushes the
  `slotArray` copy to r3 so `capacity` takes r2. A byte-offset
  `(s32)gridHeadBase + (i << 2)` fixes one `adds` operand order. See
  [docs/matching/issue-9-raw-asm-pass.md](../matching/issue-9-raw-asm-pass.md).

### Matched in the late NAKED retry 3

- `src/player/action_ctrl_hang.c` - `ActionCtrlReleaseHang` (the jump-start
  handler, issue #17), old_agbcc. The 0x600 is a reload; an r2
  register variable that only empty asms set and use keeps r2 live
  across the add, so reload spills r3 for it as the ROM does. See
  [late-naked-retry-3.md](../matching/late-naked-retry-3.md).

### Matched in the late-ROM NAKED retry

- `src/graphics/actor_part45d.c` - `LoadBgPicture` and `FillBgPictureMap`
  (the BG1 picture loader and its map repack loop, issue #56), old_agbcc
  (the object is on `OLD_AGBCC_OBJS`). 7B0 inlines a `static inline`
  copy of the loop (the ROM's 7B0 has the inlined loop's two store
  pointers); 8E8 is the loop as its own function. The map entry is read
  in two statements and the high nibble masked; 7B0 also needs three
  empty `asm("" : : "r"(x))` extra references (`dest` after the loop,
  `cols` twice before the call) to settle two register-priority ties.
- `src/menus/continue_prompt.c` - `ContinuePromptLoop` (the fade overlay's
  per-frame input driver, issue #63), old_agbcc (object added to
  `OLD_AGBCC_OBJS`). An extra reference to `audio` at the top of the
  loop replaces the old r8 pin on the pair counter, and a `"+r"` asm on
  the input copy between the `& 1` and `& 8` tests keeps their shifts
  apart.

See [docs/matching/late-rom-naked-retry.md](../matching/late-rom-naked-retry.md).

### Matched in the category-driver NAKED retry

- `src/graphics/actor_part101.c` - `InitActorCategory` (issue #48), the
  category setup + per-VBlank loading loop, old_agbcc (object added to
  `OLD_AGBCC_OBJS`). First C draft; the "four high-register pins" were
  loop.c's own hoisting. Pointer locals for the two counters assigned at
  the top of the outer loop, `&gLevelState` assigned right before
  the inner loop, an if/else exit-state chain and a volatile DMA fill
  source reproduce the ROM's reload registers and preheader order.
- `src/graphics/actor_part103.c` - `RunActorCategoryFrame` (issue #49), old_agbcc
  (the file's compiler). The sub-effect loop is a plain `while` whose
  exit test gcc copies ahead of the loop; the test is a macro (an inline
  function's block notes stop the copy) building the next-record address
  as `off`, then `base + 0x14`.

See [docs/matching/category-driver-naked-retry.md](../matching/category-driver-naked-retry.md).

### Matched in the fresh NAKED retry

- `src/crates/crate_grid_unlink.c` - `UnlinkCrateFromGrid` (the grid-removal
  primitive, issue #9), old_agbcc (the object was added to
  `OLD_AGBCC_OBJS`). The ROM sets the phase-2 bucket index to 0x100
  after each removal, so the C does that too. The free-list head is
  pinned to r1. See
  [docs/matching/fresh-naked-retry.md](../matching/fresh-naked-retry.md).

### Matched in the third issue #15/#16 NAKED retry

- `src/player/action_ctrl_update.c` - `UpdateActionCtrl` (issue #16), old_agbcc
  (the file's compiler). Each `self->part` re-read has its own local and
  both camera tests read `y` into a local first; `listCount` goes through
  an `s32` (keeps the signed `bgt`); the queued-state store is
  `ActQueue27`; the input mask is an opaque 0x100; the switch bodies are
  in ROM order. The PMF method record then gets its 8-byte stack slot
  (and the unused `r7` push) by itself.
- `src/player/action_ctrl_idle.c` - `ApplyActionCtrlMotion` (issue #16), old_agbcc
  (the file's compiler). No flag/tag pointer locals: `self->motionXPending` and
  `self->motionX` are read through `self` each time and old_agbcc's GCSE
  makes the ROM's address copies; the record lookup is
  `*(gCtrlMotionRecords + i)`; three empty `asm("" : : "r"(self))`
  extra references settle the remaining register ties.

See [docs/matching/issue-15-16-naked-retry-3.md](../matching/issue-15-16-naked-retry-3.md).

### Matched in the third near-miss sweep

- `src/player/action_ctrl_states.c` - `ActionCtrlStateCrouch` (issue #17),
  old_agbcc. The facing block's second branch writes through a scoped
  `volatile u8 *`, which keeps the `+0x28` address in the part copy's
  register ahead of the -0x11 mask.
- `src/bosses/hovercraft.c` - `ConvertHovercraftTiles` (P2 fill-level meter),
  both compilers. The 0xf mask is an opaque value
  (`asm("" : "=r"(m) : "0"(0xf))`) ANDed as `m & b`, the second byte has
  its own local, the second loop has its own counter, and the row
  header is written step by step in ROM order.

See [docs/matching/near-miss-polish-3.md](../matching/near-miss-polish-3.md).

### Matched in the second big NAKED retry

- `src/player/action_ctrl_event.c` - `ActionCtrlHandleEvent` (issue #16, 1420 bytes,
  the 25-case action dispatcher with two nested 7-case keyframe
  lookups), old_agbcc (object added to `OLD_AGBCC_OBJS`). First C
  draft; the nested lookup is a macro that assigns its destination in
  each case, the trio's `1` in the fire-button cases is an
  `asm`-initialised local kept apart from the `& 1` test, and a
  volatile read reproduces a dead `ldr` of `self->state`.

See [docs/matching/big-naked-retry-2.md](../matching/big-naked-retry-2.md).

### Matched in the fourth mid-range NAKED retry

- `src/bosses/airship_graphics.c` - `ConvertAirshipTiles` (issue #58, VRAM
  fill-level meter), both compilers. The fixes that closed its one-row
  twin `ConvertHovercraftTiles`: the 0xf mask from `asm("" : "=r"(m) : "0"(0xf))`
  ANDed as `m & b`, a separate local for the second byte, the second
  loop's own counter and its header written in ROM order, plus the
  `"+m"` height reload.

See [docs/matching/mid-range-naked-retry-4.md](../matching/mid-range-naked-retry-4.md).

### Matched in the stack-box NAKED retry

- `src/crates/crate_touch.c` - `PlayerAnimWouldTouchCrate` (issue #11, the
  "would action `x` newly hit `self`" three-box test), old_agbcc
  (`crate_touch.o` joined `OLD_AGBCC_OBJS`). Each builder call and
  the first overlap test take the player box's address through an
  empty `asm("" : "+r")` copy, so cse doesn't keep `sp+16` in a
  callee-saved register across the calls; `rec` is shared by the first
  two blocks.

See [docs/matching/sp-box-retry.md](../matching/sp-box-retry.md).

### Matched in the size2 NAKED retry

- `src/frontend/credits.c` - `LoadCreditsLogos` (issue #64, the map
  screen's popup-text asset loader), old_agbcc. A non-volatile asm with
  outputs is an ordinary expression to GCSE, so PRE hoisted even a
  `"+r"` copy of `slot`. The palette index is a copy passed through
  `asm volatile`, which GCSE never enters in its table; `slot + 1` is
  still hoisted as in the ROM. The copy is an r1 register variable with
  one extra reference after the shift, so the shift result lands in r0.

See [docs/matching/size2-naked-retry.md](../matching/size2-naked-retry.md).

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
(`MakeLinkHandshakeId`/`ResetLinkSessionState` in `src/link/link_handshake.c`) for the
established convention, and each entry's linked write-up for why
plain C didn't converge.

- **Now matched as real C (issue #9 hold pass, see Matched and docs/matching/issue-9-raw-asm-pass.md); entry kept for history.** **`UpdateCrateList`** (`src/crates/crate_list_update.c`) - a per-frame
  grid-maintenance pass over the 3-bucket window around the tracked
  sub-object's own column (plus bucket 255): lazily links newly-large
  objects into bucket 255 (`LinkCrateToActiveBucket`'s own body, inlined), removes
  and destroys objects flagged for removal, and box-tests/marks the
  rest. Fully understood; parked on a many-high-register allocation
  gap across its three inner-loop branches (a same-size C draft, 215
  halfwords off, now sits under `#if NON_MATCHING` - see
  `docs/matching/fresh-naked-retry.md`). See
  `docs/matching/naked-spatial-grid-tail.md`.
- **Now matched as real C (late NAKED retry 3, see Matched and docs/matching/late-naked-retry-3.md); entry kept for history.** **`ActionCtrlReleaseHang`** (`src/player/action_ctrl_hang.c`), GitHub issue
  #17 - the jump-start handler, whose old_agbcc C (kept under
  `NON_MATCHING`) is 3 halfwords off: the reloaded 0x600 lands in r2
  where the ROM uses r3. (`ActionCtrlStateLeftGround` from this entry is matched since
  the mix NAKED retry 5, `docs/matching/mix-naked-retry-5.md`, and
  `ActionCtrlStateCrouch` since the third near-miss sweep.) See
  `docs/matching/issue-17-0x08012fbc-actor.md`, "Third pass".
- **Now matched as real C (issue #9 NAKED retry, see Matched and docs/matching/issue-9-naked-retry.md); entry kept for history.** **`CollidePlayerWithCrates`** (`src/crates/crate_player_collide.c`) - another
  3-bucket-window grid pass, this one reading the player's state to
  dispatch `BreakCrateTouchedByPlayer`/`CollideCrateWithPlayer` per object. Fully understood;
  parked on a cross-branch register-role gap (`r8` reused for two
  different base addresses). See
  `docs/matching/naked-spatial-grid-tail.md`.
- **Now matched as real C (issue #9-#11 NAKED retry, see Matched and docs/matching/issue-9-11-box-naked-retry.md); entry kept for history.** **`sub_8009BE0`** (`src/objects/step_probe.c`) - a physics/
  collision step-probe: runs `self`'s position through `sub_8008278`,
  then probes it via `ProbeTerrain` up to 4 times (nudging Y each
  retry) before giving up. Fully understood; parked on a register-
  reload quirk in the retry loop. See
  `docs/matching/naked-spatial-grid-tail.md`.
- **Now matched as real C (stack-box NAKED retry, see Matched and docs/matching/sp-box-retry.md); entry kept for history.** **`PlayerAnimWouldTouchCrate`** (`src/crates/crate_touch.c`, GitHub issue
  #9/#10) - `PlayerHasRoomForAnim`'s only callee: builds three AABBs (the
  entry's own current hitbox, the player's current hitbox, and the
  player's hitbox for the caller's target action) via the same
  `self+0x20`-table-at-28-byte-stride "keyframe/hitbox record"
  convention `BreakCrateTouchedByPlayer` (`crate_hit.c`) documents, then returns
  whether the entry's hitbox overlaps the target-action hitbox without
  already overlapping the current one. A strict superset of
  `BreakCrateTouchedByPlayer`'s own two-AABB shape, already documented there as
  resistant to gcc 2.9 C reconstruction (the `r7`/`r8`/`sb`
  register-reuse pattern across AABB blocks) - not re-attempted as C
  here; transcribed directly as byte-exact NAKED asm instead. See
  [docs/matching/issue-9-10-0x0800aaec-graphics.md](../matching/issue-9-10-0x0800aaec-graphics.md).
- **Now matched as real C (issue #9-#11 NAKED retry, see Matched and docs/matching/issue-9-11-box-naked-retry.md); entry kept for history.** **`ProbeGroundSpriteTerrain`/`ProbeGroundSpriteFloor`** (`src/objects/ground_sprite_collide.c`,
  GitHub issue #9/#10) - the part-object movement-resolution pair
  `docs/matching/issue-9-0x08007634-actor.md` flagged as built on
  `sub_8008200`/`ProbeTerrain`/`sub_8026C3C`/`sub_8026BF8` (the first
  two now matched, see `sprite_obj.c`/`terrain_probe.c`). `ProbeGroundSpriteFloor`
  is a single Y-axis "floor" probe via `sub_8026BF8`; `ProbeGroundSpriteTerrain` is
  the larger orchestrator - two gate checks, an unconditional
  `self+0x74` zero (confirming and completing `docs/rom_map.md`'s own
  partial note: `self+0x74` accumulates which movement axes/directions
  collided this call), a fast quad-based floor/wall probe via
  `ProbeGroundSpriteFloor`/`sub_8026C3C`, then a per-axis fallback through the
  already-matched `ProbeTerrain` tile-scan API. Both keep `sb`/`sl`/`r8`
  (and, for `ProbeGroundSpriteTerrain`, `r7` too) live simultaneously across many
  `bl` calls, reused for different values block to block - the same
  `r7`/`r8`/`sb` cross-block register-reuse shape this exact ROM
  neighborhood already established as gcc-2.9-resistant (`sub_8009BE0`,
  `PlayerAnimWouldTouchCrate` above, `sub_800CEAC`/`sub_800CF70` below); confirmed
  directly via one isolated-compile attempt against `ProbeGroundSpriteFloor`
  (this compiler's natural register allocation used no high registers
  at all, a structurally different solution rather than a near-miss).
  Transcribed directly as byte-exact NAKED asm instead. See
  [docs/matching/issue-9-0x0800a178-graphics.md](../matching/issue-9-0x0800a178-graphics.md).
- **Now matched as real C (strag4 retry, see Matched); entry kept for history.** **`StartActionCtrlTornadoSpin`** (`src/player/action_ctrl_hang.c`) - a three-arm
  mgr-trampoline handler keyed on `self+0x24`/`self+0x22`, picking one
  of three table-index fallbacks. The strag2 retry left a C draft under
  `#if NON_MATCHING` that is 2 halfwords off under both compilers (one
  pointer copy and load swapped). See
  `docs/matching/strag2-naked-retry.md`.
- **Now matched as real C (strag2 retry, see Matched); entry kept for history.** **`EndActionCtrlSpin`** (`src/player/action_ctrl_moves.c`) -
  `self+0x26`/`mode`/`flags`-gated mgr-trampoline dispatcher. See
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- **Now matched as real C (strag2 retry, see Matched); entry kept for history.** **`SteerActionCtrlSpin`** (`src/player/action_ctrl_moves.c`) -
  `self+0x27`/`self+0x2b`/`mode`-gated state/counter/table-index trio
  reset, tail-calling `UpdatePlayerFacing`. See
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- **Now matched as real C (strag2 retry, see Matched); entry kept for history.** **`ActionCtrlStateBodySlamStart`** (`src/player/action_ctrl_moves.c`) -
  `part+0x38`/`HasSuperBodySlam`-gated mgr-trampoline dispatcher. See
  `docs/matching/issue-18-0x08014f8c-actor.md`.
- **Now matched as real C (size2 NAKED retry, see Matched and docs/matching/size2-naked-retry.md); entry kept for history.** **`LoadCreditsLogos`** (`src/frontend/credits.c`, GitHub issue #64) -
  the map screen's popup-text asset loader. The issue #64/#65 NAKED
  retry left a near-miss C draft under `#if NON_MATCHING` (old_agbcc):
  the "seven live values" allocation is right; what is left is that gcc
  computes the palette-slot address `slot << 5` ahead of the tile loops
  and spills it (4 bytes of extra frame), where the ROM computes it at
  the palette copy. The second retry traced the early `slot << 5` to
  GCSE's PRE, not loop.c. See
  [docs/matching/issue-64-65-naked-retry.md](../matching/issue-64-65-naked-retry.md)
  and [issue-64-65-naked-retry-2.md](../matching/issue-64-65-naked-retry-2.md).

## Parked (`NON_MATCHING`, not yet byte-exact)

### NAKED transcription (byte-exact, but not real decompiled C)

These functions produce byte-exact ROM output, but only because the
entire function body is hand-transcribed disassembly wrapped in inline
`asm()` - the C-level matching attempt failed and the raw bytes got
embedded as asm instead. They're tracked as parked, not matched.

- **Now matched as real C (issue #9-#11 NAKED retry, see Matched and docs/matching/issue-9-11-box-naked-retry.md); entry kept for history.** **`CheckSpritePickup`** (`src/objects/sprite.c`) - the player-collision
  "kind" spawner. Hits this project's confirmed categorical r7-pin
  compiler bug. GitHub issue not tracked separately, see
  `docs/matching/naked-sub_8007dbc.md`.
- **`DrawJetpackCheckpointText`** (`src/graphics/actor_anim.c`) - fixed-position
  (120, 106) OAM setup for one sprite frame - screen-space visibility
  cull, then builds the OAM attribute words and calls
  `SetupSpriteFrameOam`; near-identical twin of `DrawPolarCollectedWumpa`
  (`actor_part19b.c`, see below). Now fully matched as real C: the
  `| 0`-dead-store idiom closes via the established opaque-asm idiom,
  and the register-budget gap the original parking cited turned out to
  be an r7-pin-hazard artifact (leaving `frame` unpinned lets the
  natural allocator land it in r7 correctly) rather than a genuine
  register shortage. GitHub issue #71, see
  [docs/matching/issue-71-0x0803b060-actor.md](../matching/issue-71-0x0803b060-actor.md).
- **`DrawPolarCollectedWumpa`** (`src/graphics/actor_part19b.c`) - fixed-position
  OAM setup for one sprite frame, `DrawJetpackCheckpointText`'s twin above; now fully
  matched as real C. The dead `flag = 0` initializer closes via an
  opaque two-instruction `asm volatile` materialization (a single
  `mov r8, #0` isn't valid Thumb - only lo registers take an
  immediate `mov`), and the remaining register-choice gaps close with
  the same pin-matching techniques worked out for `DrawJetpackCheckpointText`. The
  old raw `asm/code_3_2_20_28568_c2fc.s` is retired. See
  `docs/matching.md`, issue #52.
- **`CreatePolarCollectedWumpa`** (`src/graphics/actor_part19c2.c`) - a homing/
  seek-toward-point spawn-effect constructor; now fully matched as
  real C. The Manhattan-distance abs-value computation uses the ROM's
  own branchless idiom (`(x ^ (x >> 31)) - (x >> 31)`, compiling to
  `asr`/`eor`/`sub`) rather than a `(x < 0) ? -x : x` ternary, which
  this compiler turns into a `cmp`/`bge`/`neg` branch instead. Also
  fixes a genuine correctness bug found while tightening the register
  match: pinning the `self+0x1c` reload to `r1` *before* the
  `GetActorBgCenterX()` call it's actually meant to follow let this
  compiler's optimizer silently skip the reload and reuse a stale
  register value from an unrelated earlier computation - caught by a
  direct byte compare against the ROM, not just a register-choice
  cosmetic mismatch. The old raw `asm/code_3_2_20_28568_c3e8.s` is
  retired. See `docs/matching.md`, issue #52.
- **`UpdateHovercraftCannonFlash`** (`src/bosses/hovercraft_cannon_flash.c`) - a position-sync/
  flag/trampoline updater; now fully matched as real C. The ROM
  computes a "should animate" 0/1 value and re-checks it against zero
  even though the value is a compile-time constant on each path -
  this compiler's dead-branch elimination always collapsed that
  redundant compute-then-recheck step for a plain local, closed via
  an empty `asm volatile("" : "+r"(doAnim))` making the value opaque
  right before the check. The old raw
  `asm/code_3_2_20_28568_c99c_31784_33ef4_34270.s` is retired. GitHub
  issue #63, see `docs/matching/issue-63-0x08033ef4-actor.md`.
- **`sub_8034314`** (`src/bosses/hovercraft_cannon_flash.c`) - `UpdateHovercraftCannonFlash`'s
  boolean-returning twin; matched as real C immediately, no opaque-asm
  fix needed - returning the value directly (rather than branching on
  it to decide whether to call `UpdateActor`) means there's no
  recheck for dead-branch elimination to collapse. The old raw
  `asm/code_3_2_20_28568_c99c_31784_33ef4_34314.s` is retired. GitHub
  issue #63, see `docs/matching/issue-63-0x08033ef4-actor.md`.
- **`AllocJetpackPlayerTiles`** (`src/graphics/actor_part43b.c`) - computes two
  keyframe-driven tile-cache sizes via `AllocVramTileBlock`; now fully
  matched as real C. The ROM's "materialize the multiply result, then
  copy it again before shifting" idiom (`adds r2,r3,#0; muls r2,r1,r2;
  adds r0,r2,#0; lsls r0,r0,#5`) closes via an opaque `asm volatile`
  forcing the exact register-to-register copy this compiler's
  dead-store elimination always collapsed. Closing that gap surfaced a
  further chain of register-role mismatches in the index/address
  computation (`table` needing an early load into `r3`, the `+2` index
  constant needing to be materialized via an opaque `mov #2` rather
  than a plain `register`-pinned local, and the two blocks' final
  byte-load pairs needing per-block register pins matching the ROM's
  own `ldrb` register choices), each closed with the same
  register-pin/opaque-asm technique. See
  `docs/matching/issue-56-0x0802f0dc-actor.md`.
- **`CreateJetpackShot`** (`src/graphics/actor_part45c.c`) - an
  `InitActorPart`-based constructor for this cluster's `self` object:
  forwards its first three real arguments plus one stack argument
  straight to `InitActorPart`, then marks `self+0x54` = 1, sets
  `self+0x50`'s event/trampoline table to `gJetpackShotVtable`, and
  stashes its remaining two stack arguments into `self+0x58`/`self+0x5c`;
  now fully matched as real C, closing the gap the same 7-argument
  `InitActorPart`-wrapper shape is still parked on for `CreateAirshipFireball`.
  Explicitly pinning `e`/`f` to their ROM registers (`r6`/`r7`) either
  adds a spurious extra `r8` push/pop (a relay attempt) or - for `r7`
  specifically - drops that register from the compiler's own prologue
  push/pop list outright (a genuine agbcc/gcc 2.9 Thumb-prologue bug,
  the same quirk `DestroyJetpackPlayer` above hit for a plain low-register pin).
  The fix: pin only the constant `1` to `r5`; leave `self`, the stack
  argument `d`, and both `e`/`f` completely unpinned (`self` as a plain
  `u8 *` local, `d` used directly as `InitActorPart`'s stack argument,
  `e`/`f` as plain `register` locals with no explicit hardware
  register). With that much natural register pressure, this compiler's
  own allocator picks `r4`/`r6`/`r7` for `self`/`e`/`f` on its own -
  correctly including all of `r4`-`r7` in the push/pop list - and, in
  the declaration order `self`, then the pinned constant, then `e`,
  then `f`, schedules the loads in the ROM's own self/d/e/f/constant
  order. Retires the old raw `asm/code_3_2_20_28568_c99c_2fa04.s`. See
  `docs/matching/issue-56-0x0802f0dc-actor.md`.
- **`ApplyPlayerVelocity`** (`src/player/player_update.c`, GitHub issue #9) -
  a per-frame velocity integrator moving `self+0x60`/`self+0x64`
  toward `self+0x50`/`self+0x5c` by `self+0x4c`/`self+0x58` each call,
  deriving a `self+0x24` direction-flag byte and applying the result to
  the object's position, then recording the resulting Y velocity into
  an unlabeled RAM address (`0x0300129C`); now fully matched as real C.
  The trailing `0x0300129C` block's "genuinely redundant" conditional
  store (see the file's header comment) gets proven dead by this
  compiler regardless of C-level phrasing, collapsing its guard down to
  just one half of the `&&` - closed by emitting the whole load/
  compare/branch/store sequence verbatim via one opaque `asm volatile`
  block instead of fighting the optimizer's proof, which also
  reproduces the ROM's own address-in-`r0`/value-in-`r2` register
  choice directly. Also found and fixed a genuine gcc-2.9 register-pin
  miscompile while closing this: pinning both `vx` and `vy` (the X/Y
  velocities, needed in `r3`/`r1` to match ROM) at once made the
  function's own `return (vx != 0 || vy != 0)` fold to an unconditional
  `mov r0, #1` - fixed by pinning only `vx`, leaving `vy` an unpinned
  local (it lands in `r1` naturally anyway). Retires the raw
  `asm/code_3_2_16_b270.s`. See
  `docs/matching/issue-9-0x08007634-actor.md`.
- **`ApplySpriteVelocity`** (`src/objects/moving_sprite.c`, issue #97) - a
  velocity/position integrator: steps each axis's velocity toward its
  max by its accel amount (clamped so it never overshoots), builds a
  direction-flags byte from the clamped velocities' signs, caches the
  pre-move position, applies the velocity, and updates a global
  (`gLastSpriteVelY`) with the Y velocity; now fully matched as real
  C. Closed using the exact fix worked out for its near-identical twin
  `ApplyPlayerVelocity` above (same per-axis clamp shape, same field offsets):
  `vx` pinned to `r3` while `vy` stays unpinned, and the trailing
  global-update block's "genuinely redundant" conditional store emitted
  verbatim via one opaque `asm volatile` block instead of fighting this
  compiler's dead-store-elimination proof, reproducing the ROM's own
  address-in-`r0`/value-in-`r2` register choice directly. Retires the
  old raw `asm/code_3_2_9.s`. See
  [docs/matching/issue-97-ApplySpriteVelocity.md](../matching/issue-97-ApplySpriteVelocity.md).
- **`InitStarfield`** (`src/frontend/starfield.c`) - constructs the
  particle-trail BG0 object; now fully matched as real C. The ROM
  builds a 4-bit-palette-bank tile-index mask (0xFFFFF000) by loading
  the literal into `r1` first and copying it into `r5` (`ldr
  r1,=0xFFFFF000; adds r5,r1,#0`), rather than the single direct `ldr`
  a plain `mask = -0x1000;` compiles to - closed by pinning an
  intermediate local to `r1` (its initializer must stay a plain C
  constant, not an inline-asm-embedded immediate, so the value stays in
  the compiler's own literal pool at the ROM's actual pool position
  rather than becoming a second, separately-pooled literal appended
  after it) and forcing the `r1`->`r5` copy via `asm volatile`. Also
  needed the `mapBase + (row << 6)` addition's operand order pinned the
  same way, and the `col = 0x1d` initializer moved after that
  computation in the C source (gcc otherwise schedules a trivial
  immediate move ahead of a nearby pinned-register asm block by its
  literal source position). Retired the multi-function raw
  `asm/code_3_2_20_28568_c99c_31784_33ef4_34374.s`, split at the time
  into `asm/code_3_2_20_28568_c99c_31784_33ef4_34480.s` (real bytes for
  the twin `DrawStarfield`) - that fragment is now retired too (see
  below), closing `starfield.c` entirely. GitHub issue #63, see
  `docs/matching/issue-63-0x08033ef4-actor.md`.
- **`DrawStarfield`** (`src/frontend/starfield.c`) - `InitStarfield`'s
  companion per-frame updater; now fully matched as real C, closing the
  file. Commits last frame's `tileBuffer` to tile VRAM, clears it back
  to zero, then for each active particle inlines the same nibble-
  address formula as `PlotStarfieldPixel` (`starfield.c`) *twice* (nibble
  `1` pre-move, nibble `2` post-move), applying `dx`/`dy` and
  respawning via `SpawnStar` in between. Needed a mix of
  `PlotStarfieldPixel`'s own three fixes (unsigned casts for the `x`/`newX`
  bounds checks alongside plain signed `>> 11` block-index shifts,
  `addr`'s two halves split into separate statements, and one opaque
  `asm volatile` for the `bic`/`orr`/`strh` tail - simpler here than
  `PlotStarfieldPixel`'s own tail since this function's ROM build never needs
  an extra materialize-then-copy-back step) plus two more scheduling-
  order fixes this larger, twice-inlined function surfaces on its own:
  `x`'s raw value and its `>>8` pixel value must be computed
  immediately, before `y` is even loaded (same for `blockX`'s `<<6`
  term before `blockY` loads), the post-move `slot->x = newX` store
  must happen immediately after computing `newX` rather than batched
  with the `y` store, and the second inlined copy's `oldVal` (pinned to
  `ip`) must be assigned by a plain statement after the position
  reload rather than as its `register` declaration's own initializer.
  Retires `asm/code_3_2_20_28568_c99c_31784_33ef4_34480.s` entirely.
  GitHub issue #63, see `docs/matching/issue-63-0x08033ef4-actor.md`.
- **`SpawnStar`/`PlotStarfieldPixel`** (`src/frontend/starfield.c`) - a
  128-slot particle-slot spawner (rolls two `RandRange` random values
  against the 256-entry `gSineTable` direction table to seed a
  position/velocity record) and a 4-bit-per-cell tilemap nibble writer;
  now fully matched as real C. `SpawnStar`'s previously-parked
  register-allocation gap for the final multiply/shift turned out not to
  need any register pins or opaque asm at all: this compiler's codegen
  for `dest = a * b` always materializes/copies the *left* operand into
  the destination register before the `muls`, and the ROM's own build
  happens to write the table lookup as the left operand
  (`table[...] * speed`) rather than the speed value - simply writing the
  C multiplication in that same order matched immediately.
  `PlotStarfieldPixel`'s residual `addr`/`blockY` register-role gap turned out
  to be three separate, independently-found issues: `x`'s bounds check
  wants an unsigned compare but `x >> 3` wants a *signed* arithmetic
  shift (modeled with an explicit `(s32)x >> 3` cast), `addr`'s two
  halves needed splitting into separate statements to pin gcc's
  operand-evaluation order (a combined `a + b` expression let it
  evaluate the blockY half first, opposite the ROM's x-half-first
  order), and the temporary `mask` needed to stay a plain 32-bit type
  (`u32`, not `u16`) to avoid a spurious 4-instruction 16-bit-truncation
  sequence this compiler otherwise inserts around `0xf << shift` (the
  low 16 bits are all `bics`/`orrs` ever reads, so the truncation was
  never actually needed). The final residual gap - the ROM's own
  "materialize `cell`, `bics` it, then copy the result back before
  `orrs`/`strh`" idiom, the same class of redundant-copy-after-a-binary-op
  quirk already seen for `AllocJetpackPlayerTiles`'s multiply - closed with one
  opaque `asm volatile` block reproducing that exact instruction
  sequence, taking `shift` and a `register ... asm("r2")`-pinned
  `tileMapEntry` as inputs and the already-`r3`-pinned `val` as an
  in/out operand. Retires
  `asm/code_3_2_20_28568_c99c_31784_33ef4_345b0.s` entirely. GitHub
  issue #63, see `docs/matching/issue-63-0x08033ef4-actor.md`.
- **`CreateHovercraftSideGun`** (`src/bosses/hovercraft_side_gun.c`, GitHub issue #63) -
  an `InitActorPart`-based constructor with a trailing (stack-passed,
  byte-sized) 6th argument and a spawn-record ternary; now fully
  matched as real C. Two gaps, both closed with `asm volatile`
  anchors: the 6th argument needs the same stack-slot-address-then-
  `ldrb` anchor already established for other trailing byte arguments
  (see `DrawSaveMenuMain` in `issue-5-overlay-ui-sync.md`), materialized
  into a `register u32 asm("r9")` pin mirroring the ROM's own `sb`
  cache; and the `self+0x5c` spawn-record ternary needs the ROM's
  genuine two-way branch diamond (a forward `beq`/`ldr`/`b` skipping a
  computed false branch, with two literals pooled together right after
  the skip) rather than the eager "compute one value, conditionally
  overwrite" shape this compiler always produces from a plain ternary
  or if/else - and, separately, forcing that diamond through any
  register-pinned C-level if/else made this compiler's own parameter-
  homing pass reorder `self`'s prologue copy relative to
  `part`/`b`/`cParam` (still the correct register, just the wrong
  instruction position), a discrepancy that didn't respond to any
  combination of pinning/goto/precomputed-address tried. Closed by
  moving the diamond into one opaque `asm volatile` block referencing
  `self`'s known `r5` home directly by name (not as an operand, which
  is what avoids perturbing the surrounding allocation) with a real
  `ldr r0, =0xFFFFBF00` assembler pseudo-op and a manual `.pool`
  directive right after the skip branch - and moving the preceding
  `gHovercraftSideGunVtable` store into its own tiny `asm volatile` island
  too, since a real, respected `.pool` split only works for symbols
  whose literal load is itself opaque assembler text, the same gap
  already documented for `UpdateActorPaletteCycle` in `actor_part53.c`. Retires
  the raw `asm/code_3_2_20_28568_c99c_31784_33ef4_34058.s`. See
  `docs/matching/issue-63-0x08033ef4-actor.md`.

- **Now matched as real C (last-four NAKED retry, see Matched and docs/matching/last-four-naked-retry.md); entry kept for history.** **`CollidePlayer`** (`src/player/player_collide.c`, GitHub issue
  #9/#10; NAKED in that file since the issue #9 raw-asm pass, which also
  replaced the draft - see `docs/matching/issue-9-raw-asm-pass.md`) - a per-frame
  reentrancy-guard-shaped wrapper dispatching a pending-action "kind"
  byte (`gLevelLayers+0x29`) through a 10-case jump table, then a
  keyframe-lookup/camera-position probe via `GetSpriteFrame`/
  `GetTerrainFlagsAt` sharing `GetSpriteFrameAnchor`'s case-to-block mapping. Every
  load/store/branch/call confirmed correct against the ROM, and now
  (a second follow-up session, 98.0% fuzzy-matched, up from 96.8%)
  register-for-register byte-exact almost everywhere: both jump
  tables, all 10 case bodies (two needing `asm volatile` islands to
  reproduce a ROM cross-case tail-merge), the camera-probe tail's
  Y-snap arithmetic (closed this session - a `(masked + 7) - y`
  expression was getting re-associated into a different register
  pairing than the ROM's own), and `kindZero`'s `self+0x68 == 8` test
  (also closed this session, via a matching-constraint `asm volatile`
  reusing `p68`'s own already-`r7` allocation rather than forcing a
  brand-new register binding) all match. One narrow, purely
  register-*choice* gap remains (the `self+0x105` clear's scratch-
  register pick, `r0` here vs. the ROM's `r2` - reconfirmed resistant
  to every register-pin/`asm` variation tried across two sessions now,
  without perturbing other, already-matching code, the "ripple"
  effect), plus one small single-instruction side effect of closing
  the `kindZero` gap (a redundant `movs r1, #0` the compiler schedules
  from provable-constant-propagation that no placement of the source
  assignment moved or eliminated). See
  [docs/matching/issue-9-10-0x0800a884-graphics.md](../matching/issue-9-10-0x0800a884-graphics.md).
- **Now matched as real C (issue #9 NAKED retry, see Matched and docs/matching/issue-9-naked-retry.md); entry kept for history.** **`PlayerHasRoomForAnim`** (`src/player/player_anim_room.c`, GitHub issue
  #9/#10) - the input-action-check function the 42-slot
  `gActionCtrlStateTable` action-dispatch table's entries call: gates a
  `ProbeTerrain` proximity probe against the player, then walks
  `gCrateList`'s object list, testing each entry via
  `_call_via_r1` and calling `PlayerAnimWouldTouchCrate` (now examined and closed,
  `crate_touch.c`) on a hit. Every instruction matches except one
  5-instruction pair (the `gCrateList` list-walk's loop-
  condition-check/loop-entry register roles - the ROM re-loads
  `&gCrateList` from the literal pool fresh every iteration,
  landing it in `r0` and reusing that register as the dereferenced
  value in place; no C phrasing tried reproduced that exact
  per-iteration reload shape without either losing the reload
  entirely via gcc's own loop-invariant hoisting, or costing an extra
  callee-saved register). Default build uses a byte-exact NAKED
  transcription instead - see
  [docs/matching/issue-9-10-0x0800aaec-graphics.md](../matching/issue-9-10-0x0800aaec-graphics.md).
- **`SetCtrlTargetMotionY`/`StartCtrlTargetMotionY`** (`src/player/player_flags.c`) -
  mirror-flag-gated 3-vector copies. This compiler unconditionally
  spills the `vec` pointer to a callee-saved register (`push
  {r4,lr}`/`pop {r4}`) whenever it's referenced in both branches of an
  if/else, even with nothing to clobber it - the ROM is a true leaf
  function using only r0-r3. Three independent fixes (pinning `self`
  alone; also pinning/reassigning `vec`; restructuring into a
  `goto`-based flow) all produced an identical 8-byte-larger result -
  see `docs/matching.md`, "A new unnamed object:
  `player_update.c`/`player_flags.c`".
- **Now matched as real C (issue #9 NAKED retry, see Matched and docs/matching/issue-9-naked-retry.md); entry kept for history.** **`GetSpriteBounds`/`GetSpriteHitbox`** (`src/objects/sprite.c`),
  **`AdvanceSpriteAnim`** (`src/objects/sprite.c`), and
  **`UpdatePartList`/`CollidePartList`/`CollidePartWithPlayer`**
  (`src/objects/sprite_anim.c`) / **`CollidePartWithObject`**
  (`src/objects/part_collide.c`) - stayed parked here for a long time
  on exactly the register-canonicalization/stack-layout classes of gap
  documented at length throughout this page (`r7`-pin hazards and a
  C-inexpressible stack-layout coincidence). All seven are `NAKED`
  functions whose bodies are a literal instruction-for-instruction
  transcription of the ROM's own assembly (byte-exact, confirmed via a
  full clean `make compare`), rather than a derived C reconstruction -
  see `docs/matching/naked-oam-actor-part-batch.md`. Per project
  policy, a NAKED transcription standing in for a substantial
  function's register-allocation gap doesn't count as "matched" the way
  real decompiled C does, so all seven stay filed here rather than in
  "Matched" above, and `tools/report_units.py` tracks their address
  ranges as unmatched (`base_object: None`). `IsSpriteAnimLooping`
  (`src/objects/sprite_obj.c`) and `sub_8008188`/`sub_8008200`/
  `sub_8008278`/`GetSpriteFrame` (`src/objects/sprite_obj.c`), which shared this list in earlier versions of this
  page, were converted back to real C and matched in later sessions -
  see "Matched" above and `docs/matching.md`'s "Parked, not matched:
  IsSpriteAnimLooping"/"Parked, not matched: sub_8008188" entries (and the
  latter's siblings) for those accounts.
- **Now matched as real C (issue #9-#11 NAKED retry, see Matched and docs/matching/issue-9-11-box-naked-retry.md); entry kept for history.** **`InitCrateList`** (`src/objects/part_list.c`) - initializes a
  fixed-slot object-pool manager struct: two big 256-word zeroed
  tables (likely a pair of spatial-partition/collision grids), plus a
  singly-linked free list built over an allocated node array. Every
  load, store, and field offset confirmed correct; hits a many-register
  (item count, two persistent field addresses, a reused loop index, a
  running byte offset) allocation gap across `r3`/`sb`/`sl`/`r4`/`r8`
  that no C-level reconstruction reproduced, so converted to `NAKED`
  the same way as `CollideCrateGridPartWithPlayer`/`CollideCrateGridPartWithObject` below: a literal
  instruction-for-instruction transcription of the ROM's own assembly
  (byte-exact, confirmed via a full clean `make compare`), rather than
  a derived C reconstruction, compiled directly into `part_list.o`
  alongside the matched functions above it (no separate object needed,
  same as `AdvanceSpriteAnim`/`sprite.c`'s precedent for a NAKED
  function sharing a file with matched ones). Per project policy this
  doesn't count as "matched" the way real decompiled C does, so it
  stays filed here rather than in "Matched" above - see
  `docs/matching/naked-sub_8008f20-ResetCrateList-freelist.md`.
- **Now matched as real C (issue #9 NAKED retry, see Matched and docs/matching/issue-9-naked-retry.md); entry kept for history.** **`CollideCrateGrid`** (`src/crates/crate_grid_collide.c`) - the spatial-
  hash-grid-cluster analog of `CollidePartList`: the same grid-iteration
  shape as `DrawCrateList`, dispatching each hit to `CollideCrateGridPartWithPlayer`/
  `CollideCrateGridPartWithObject` exactly like `CollidePartList` dispatches to
  `CollidePartWithPlayer`/`CollidePartWithObject`. Every branch, field offset, and call
  argument was confirmed correct, but the `#if NON_MATCHING` C
  reconstruction's compiled size stayed larger than the real ROM
  function (a bigger gap than the established `boxH` issue alone), so
  converted to `NAKED`: a literal instruction-for-instruction
  transcription of the ROM's own assembly (byte-exact, confirmed via a
  full clean `make compare`), both grid passes byte-identical to each
  other. Per project policy this doesn't count as "matched" the way
  real decompiled C does, so it stays filed here rather than in
  "Matched" above - see `docs/matching.md`, "Parked, not matched:
  `CollideCrateGrid`".
- **Now matched as real C (issue #9 NAKED retry, see Matched and docs/matching/issue-9-naked-retry.md); entry kept for history.** **`CollideCrateGridPartWithPlayer`** (`src/crates/crate_grid_collide.c`) - `CollidePartWithPlayer`'s
  twin: byte-identical collision-hit resolution logic, operating in
  this spatial-hash-grid cluster instead of the plain array manager -
  in fact its instruction stream is byte-identical to `CollidePartWithPlayer`'s,
  down to the label offsets. Same `boxH` stack-layout gap as
  `CollidePartWithPlayer`/`CollidePartWithObject`/`CollideCrateGridPartWithObject`, so converted to `NAKED`
  the same way: a literal instruction-for-instruction transcription of
  the ROM's own assembly (byte-exact, confirmed via a full clean `make
  compare`), rather than a derived C reconstruction. Per project
  policy this doesn't count as "matched" the way real decompiled C
  does, so it stays filed here rather than in "Matched" above - see
  `docs/matching.md`, "Parked, not matched: `CollideCrateGridPartWithPlayer`".
  `CollidePartWithPlayer` itself (`src/objects/sprite_anim.c`) is unaffected by
  this change and remains its own separate `NAKED` function.
- **Now matched as real C (issue #9-#11 NAKED retry, see Matched and docs/matching/issue-9-11-box-naked-retry.md); entry kept for history.** **`ResetCrateList`** (`src/crates/crate_list_reset.c`, new file - its
  real ROM address, `0x08009914`, doesn't sit adjacent to
  `part_list.c`'s own functions, the same reason `CollideCrateGridPartWithPlayer`
  above got its own `crate_grid_collide.c` (named "i" - `CollideCrateGrid`
  claimed `crate_grid_collide.c`, the now-matched `LinkCrateToActiveBucket` claimed
  `crate_grid_link.c`, and the now-matched `DrawCrateList` claimed
  `crate_list_draw.c`, all in parallel PRs merged first)) - resets a
  pool manager to empty: tears down every active object, then rebuilds the grid and
  free list from scratch. The teardown loop was confirmed correct on
  its own; the rebuild loop is a byte-for-byte copy of `InitCrateList`'s
  own tail and hit the identical many-register allocation gap, closed
  via the same `NAKED` transcription technique - a literal
  instruction-for-instruction transcription of the ROM's own assembly
  (byte-exact, confirmed via a full clean `make compare`), rather than
  a derived C reconstruction. Per project policy this doesn't count as
  "matched" the way real decompiled C does, so it stays filed here
  rather than in "Matched" above - see
  `docs/matching/naked-sub_8008f20-ResetCrateList-freelist.md`.
- **Now matched as real C (issue #9 NAKED retry, see Matched and docs/matching/issue-9-naked-retry.md); entry kept for history.** **`CollideCrateGridPartWithObject`** (`src/crates/crate_list.c`) - `CollidePartWithObject`'s
  twin: byte-identical in shape (same collision-hit-resolve logic,
  same "dead read" trampoline call), called from elsewhere in this
  cluster - in fact its instruction stream is byte-identical to
  `CollidePartWithObject`'s, down to the label offsets. Same `boxH` stack-layout
  gap, so converted to `NAKED` the same way: a literal
  instruction-for-instruction transcription of the ROM's own assembly
  (byte-exact, confirmed via a full clean `make compare`), rather than
  a derived C reconstruction. Per project policy this doesn't count as
  "matched" the way real decompiled C does, so it stays filed here
  rather than in "Matched" above - see `docs/matching.md`, "Parked,
  not matched: `CollideCrateGridPartWithObject`". `CollidePartWithObject` itself
  (`src/objects/part_collide.c`) is unaffected by this change and
  remains its own separate `NAKED` function.
- **Now matched as real C (strag2 retry, see Matched); entry kept for history.** **`HovercraftCannonStateFire`** (`asm/code_3_2_20_28568_c99c_31784_339dc.s`, C in
  `src/bosses/hovercraft_cannon.c`) - a proximity-triggered effect/hazard
  detector measuring `self`'s distance to the player after syncing to
  the singleton's position. Every load/store, branch and call
  confirmed correct; parked on a residual register-allocation gap for
  one 16-bit constant materialization this compiler won't place in the
  ROM's chosen scratch register without breaking the surrounding
  `ip`/`r8`/`r9` pins - see
  `docs/matching/issue-62-0x08033804-actor.md`, issue #62.
- **Now matched as real C (strag2 retry, see Matched); entry kept for history.** **`HovercraftLauncherStateLaunch`** (`asm/code_3_2_20_28568_c99c_31784_33cf8.s`, C in
  `src/bosses/hovercraft_launcher.c`) - `HovercraftCannonStateFire`'s sibling proximity/
  spawn detector. Every branch and call confirmed correct; parked
  because this agbcc build never emits a callee-save push/pop for a
  plain low-register (`r0`-`r7`) `register` variable used across a
  call unless another high register is *also* live in the same
  function (confirmed with an isolated test) - pinning `self+0x64`'s
  cache to `r7` here (matching the ROM) would silently corrupt the
  caller's `r7` - see `docs/matching/issue-62-0x08033804-actor.md`,
  issue #62.

## Left raw (not attempted, or attempted and set aside)

- ~~**`DrawAffineSpritePieces`**~~ (`src/gfx/affine_sprite_pieces.c`, ROM 0x08007634,
  GitHub issue #9) - real GBA hardware-affine sprite-matrix setup.
  Moved out of `asm/code_3_2.s` as NAKED in the issue #9 raw-asm pass,
  now matched as real C under old_agbcc - see "Matched in the
  DrawAffineSpritePieces retry" above.
- ~~**`PlayerHandleEvent`**~~ (ROM 0x0800AC2C, GitHub issue #9/#10) - matched
  as real C under old_agbcc in `src/player/player_event.c` (issue #9
  raw-asm pass, `docs/matching/issue-9-raw-asm-pass.md`);
  `asm/code_3_2_16_ac2c.s` is gone.
- ~~**`CheckPlayerCtrlTurn`**~~ (ROM 0x08016048, GitHub issue #19) - matched
  as real C with issue #20 in `src/player/swim_ctrl.c` (listed
  under `graphics.md`) - see `docs/matching/issue-20-player-ctrl.md`.
