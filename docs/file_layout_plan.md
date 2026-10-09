# Source file layout plan (#575)

This is the plan for #575: give every `src/` file a name that says what it
holds, put the files in subsystem directories, and merge neighbours where that
is provably byte-safe.

**Status: complete.** The plan landed in five PRs (#596-#600, see
"Batching order" below). Every file has moved, all 66 merge groups landed,
and none had to stay separate: the 367 game code files are now 241, as
planned. `src/graphics/` and the numbered `actor_part*`, `game_loop*`,
`settings_menu*` and `graphics_loading_*` names are gone. The sections
below describe the plan as it was written, before the move; "today" means
before #596.

The per-file mapping is [`tools/file_layout_plan.tsv`](../tools/file_layout_plan.tsv):
one row per pre-move source file, in ROM (ldscript) order, with
`old_path`, `new_path`, `merge_group` and a `reason` that names the
functions the file holds. It stays in the tree as the permanent old-to-new
lookup for older docs, issues and PRs that use the old names.
`tools/apply_file_layout.py` is the script that applied it, one batch of
directories at a time. It was removed once the last batch landed (#578);
commit `805d10a0` has its final version.

The libraries (GAX2, AgbEeprom, libgcc and the BIOS SWI wrappers) already
live in `lib/` since #573 (see [`docs/libraries.md`](./libraries.md)). This
plan covers only what is left in `src/`.

## Where the files were before the move

| | Files | Notes |
|---|---:|---|
| Game code (`.text`) | 367 | 198 `actor_part*`, 56 `game_loop*`, 29 `settings_menu*`, 13 `graphics_loading_*`, 9 `hud_icon_widget*`, ... |
| Data (`.rodata`, `src/data/`) | 73 | |
| IWRAM image data | 1 | `src/iwram/iwram_data.c` |
| **Total** | **441** | |

Most code files are named after where they were matched rather than what
they hold. `src/graphics/` holds the player, crates, enemies, bosses and
menus, and `src/system/game_loop*.c` holds crates and pickups. Today's
directories are not subsystems.

## Proposed directory structure

One directory level under `src/`, as now: the Makefile's
`C_SRCS := $(wildcard $(C_SUBDIR)/*/*.c)` keeps working unchanged.

| Directory | What goes there | Files now → after |
|---|---|---:|
| `src/system/` | Boot helpers, `AgbMain`, the heap (`mem_*`), IRQ setup, input, asset loading, `MainLoop` | 7 → 7 |
| `src/util/` | Fixed-point math, number/time formatting, `printf`, the libc-style string functions, `rand`, Bresenham lines, AABB helpers | 10 → 10 |
| `src/text/` | Fonts (`Font*`), word-wrapped text and text boxes | 10 → 8 |
| `src/gfx/` | Fades, display control, palette cycles, the OAM buffer/VRAM/palette cache (`graphics.c`), sprite-piece drawing, graphics packages, the sprite-frame VRAM pool, the bitmap screen | 16 → 10 |
| `src/audio/` | The game-side audio layer over GAX2: music player, SFX, `AudioContext`, the music VCount IRQ. GAX2 itself is `lib/gax/`. | 4 → 1 |
| `src/link/` | The link-cable SIO session | 4 → 4 |
| `src/save/` | EEPROM save data, save transfer over link, the save menu | 12 → 7 |
| `src/objects/` | The side-view C++ object framework: sprite objects, part lists, moving/ground sprites, the `Ctrl` base, effect controller, collision queue, platforms, contact probes | 25 → 20 |
| `src/player/` | The player object, the action controller (moveset states), swim and input controllers | 32 → 23 |
| `src/enemies/` | The enemy controller and its behaviours | 12 → 6 |
| `src/crates/` | The crate list and grid, crate behaviours, slot crates | 30 → 19 |
| `src/pickups/` | Wumpa, extra lives, the stopwatch | 4 → 3 |
| `src/bosses/` | Mega Mix, Tiny, Cortex, Dingodile, N. Gin's airship, the hovercraft | 49 → 23 |
| `src/level/` | Level state and game context, rooms, the `Spawn*` entity spawners, BG layers, terrain and collision, the camera | 41 → 33 |
| `src/cutscene/` | The slideshow and cutscene player | 5 → 3 |
| `src/hud/` | The HUD | 7 → 6 |
| `src/menus/` | Pause menu, power dialog, level select, continue prompt | 25 → 17 |
| `src/frontend/` | Title screen, company logos, language select, starfield, credits | 11 → 7 |
| `src/actor/` | The actor-category engine behind the polar and jetpack levels: category init/select/frame, cell-animated BGs, the actor base and factory, BG pictures | 27 → 12 |
| `src/vehicle/` | Polar (riding) and jetpack levels: their players, objects and crates, and the yeti chase | 34 → 20 |
| `src/iwram/` | ARM IWRAM code and data. Unchanged. | 3 → 3 |
| `src/data/` | ROM data. Names unchanged (see below). | 73 → 73 |

If every merge below lands, 367 game code files become 241. With only the
tier-1 merges, they become 290. (All of them landed: 241.)

`src/actor/` and `src/objects/` are separate on purpose. `actor` is this
project's name for the pseudo-3D category engine (`struct actor_self`,
`InitActorPart`, `gActorCategory`). `objects` is the side-view sprite-object
framework the main platforming levels use.

## Naming convention

- **Path:** `src/<subsystem>/<subject>[_<role>].c`, lowercase snake_case.
- **Subject:** the class or feature the file implements, spelled as its
  functions spell it: `CrateList*` → `crate_list`, `ActionCtrl*` →
  `action_ctrl`, `PauseMenu*` → `pause_menu`, `Yeti*` → `yeti`.
- **Role suffix:** used only when one subject spans several files, which
  happens because of flag boundaries or the blockers listed below. Take a
  role noun from the file's own functions: `_init`, `_update`, `_draw`,
  `_states`, `_collide`, `_loop`, `_graphics`, `_create`. Never use a
  numeric suffix or a ROM address in a code file name.
- **Mixed files:** a file that holds more than one subject is named after
  the subject with the most functions. Its header comment and its TSV
  `reason` list the rest. These files are listed under "Split candidates"
  below.
- **Header comment:** each moved file should open with one line saying
  what it holds, e.g. `/* The crate list: Draw/Update/Reset ... */`. The
  existing long matching notes stay below that line.
- **Data files keep their names.** `src/data/<content>_<romaddr>.c` is
  already descriptive. The address suffix means something there, because
  each file is one ROM range interleaved with `data/data.s`, and it tells
  apart the files that share a content name (4× `actor_pmf_*`, 4×
  `bg_package_*`, 3× `entry_set_*`).
- **Libraries:** `lib/<name>/` (#573), outside this plan.

## Merge rules

Adjacent files may become one translation unit only when **all** of these
hold:

1. **Same subsystem and same subject.** The target path is the same.
2. **Contiguous in ROM.** The files are consecutive `.text` lines in
   `ldscript.txt`, with nothing in between, and they are concatenated in
   that order. gcc 2.9 emits functions in definition order, so the merged
   object lays out exactly as the separate objects did.
3. **Identical compiler and flags.** The files have the same membership in
   `OLD_AGBCC_OBJS`, `O1_OBJS`, `NO_STRENGTH_REDUCE_OBJS`,
   `NO_INTERWORK_OBJS`, `NO_RERUN_LOOP_OPT_OBJS` and `ARM_OBJS`. Flags are
   per object, so a flag boundary is never merged across. In this plan 111
   of the 190 adjacent pairs that share a directory but not a file are flag
   boundaries.
4. **Nothing at file scope collides.** Every file brings both its own
   declarations and those of the headers it includes, so each pair was
   checked against the other file's local and header declarations:
   - **Blocking** (the files stay separate):
     - an `extern` variable declared with different types, e.g. `gPlayer`
       as `void *`, `u8 *`, `struct box_part *` or `struct player *`;
     - prototypes that differ in a non-pointer type (`u8` vs `s32`,
       `void` vs `void *` return);
     - `asm(".set ...")` aliases that disagree;
     - non-static `inline` definitions, which could start being inlined
       across the old file boundary.

     Unifying these needs a source change that can alter codegen, so it
     is out of scope for a move.
   - **Mechanical** (allowed in the move, codegen-neutral):
     - delete one copy of an identical duplicated `struct` or
       `static inline` (a redefinition in one TU is a compile error);
     - rename one of two same-named local struct tags or `static inline`
       helpers that differ;
     - keep one prototype where two differ only in pointer types (pointer
       arguments pass the same way, and a pointer conversion emits no
       code);
     - add `asm(".pool")` at the old file end when the file's inline asm
       uses gas `ldr rN, =sym` with no `.pool` after it. Without it, gas
       flushes those literals at the end of the *merged* file. The trial
       below hit exactly this in `game_loop34.c` (`OpenLifeCrate`).
   - **Not a problem:** a trailing file-scope `asm(".align 2, 0")`. agbcc
     emits `.align 2, 0` before every function and the linker fills
     between objects with 0 (`} = 0`), so the padding comes out the same.
     The line stays where it is in the concatenated file.
     `#if NON_MATCHING` blocks move with their functions.

Merges are **tier 1** when the files concatenate verbatim, and **tier 2**
when they need the mechanical edits above. Every merge is still verified by
a full clean `make compare`.

### Trial run

The whole tier-1 plan was tried in a scratch copy of `origin/main` from
before #573 (with
`tools/agbcc` and `baserom.gba`): all 361 renames, all 43 tier-1 merges and
one tier-2 merge (M22, with its `asm(".pool")`). The tier-2 groups were
renamed but left unmerged. The trial updated `ldscript.txt`, the Makefile
flag lists and `tools/report_units.py`, then ran
`rm -rf build ... && make compare`, which printed
`crashbandicootxs.gba: OK`. The first attempt, without the `.pool`, differed
in 242 bytes: 238 in `crate_stack.o`, where `OpenLifeCrate`'s literal moved
to the end of the merged file, and 4 in three callers' `bl` offsets. That
is where the `.pool` rule comes from. The trial's report run reused the
`make compare` objects, so it doesn't count as a report check (see
"Verification" below). The move PRs run the report from a clean tree.
After #573 the TSV was regenerated against the new tree and every check
re-run. The plan didn't change: same paths, same 66 groups, same blockers.

### Merge groups

66 groups: 43 tier 1 and 23 tier 2. They fold 192 files into 66. "Flags"
is the per-object list membership; `default` means none.

| # | Tier | New file | Merged from (ROM order) | Flags | Edits needed when merging |
|---|---|---|---|---|---|
| M1 | 1 | `src/gfx/fade.c` | `graphics/fade_util.c`, `graphics/palette_blend.c` | default | none: concatenate verbatim |
| M2 | 1 | `src/audio/audio.c` | `audio/music_player.c`, `audio/sfx_ambient.c`, `audio/audio_context.c`, `audio/music_irq.c` | default | none: concatenate verbatim |
| M3 | 2 | `src/save/save_data.c` | `graphics/settings_menu8d.c`, `graphics/settings_menu8e.c`, `graphics/settings_menu8.c` | default | prototype EraseSaveSlot differs only in pointer types; prototype UpdateSaveChecksum differs only in pointer types |
| M4 | 1 | `src/save/save_menu_input.c` | `graphics/settings_menu8b.c`, `graphics/settings_menu8c.c` | default | none: concatenate verbatim |
| M5 | 1 | `src/save/save_menu_ui.c` | `graphics/settings_menu2.c`, `graphics/settings_menu23.c`, `graphics/settings_menu3.c` | default | none: concatenate verbatim |
| M6 | 1 | `src/menus/pause_menu_draw.c` | `graphics/settings_menu17.c`, `graphics/settings_menu21.c` | default | none: concatenate verbatim |
| M7 | 2 | `src/menus/pause_menu_widgets.c` | `graphics/settings_menu16.c`, `graphics/settings_menu7.c`, `graphics/settings_menu5.c`, `graphics/settings_menu9.c` | default | prototype FormatDecimal differs only in pointer types |
| M8 | 2 | `src/menus/pause_menu_pages_draw.c` | `graphics/settings_menu11.c`, `graphics/settings_menu12.c` | default | tag struct icon_pos duplicated (identical: delete one copy); tag struct pause_menu clashes (different body: rename one) |
| M9 | 2 | `src/menus/power_dialog.c` | `graphics/settings_menu14.c`, `graphics/settings_menu13.c` | default | tag struct power_dialog clashes (different body: rename one) |
| M10 | 2 | `src/objects/sprite.c` | `graphics/actor_part.c`, `graphics/actor_part2.c`, `graphics/actor_part3.c` | old_agbcc | prototype SetAabbPos differs only in pointer types; prototype SetAabbSize differs only in pointer types; tag struct aabb clashes (different body: rename one) |
| M11 | 2 | `src/objects/sprite_obj.c` | `graphics/actor_part4.c`, `graphics/actor_part5.c`, `graphics/actor_part6.c` | default | prototype GetSpriteFrame differs only in pointer types |
| M12 | 2 | `src/crates/crate_grid_collide.c` | `graphics/actor_part11f.c`, `graphics/actor_part11e.c` | old_agbcc | prototype CollideCrateGridPartWithPlayer differs only in pointer types |
| M13 | 1 | `src/player/player_event.c` | `graphics/actor_part81.c`, `graphics/actor_part111.c` | old_agbcc | none: concatenate verbatim |
| M14 | 1 | `src/player/player_update.c` | `graphics/actor_part49.c`, `graphics/actor_part15.c` | default | none: concatenate verbatim |
| M15 | 1 | `src/enemies/enemy_motion.c` | `graphics/actor_part114.c`, `graphics/actor_part119.c`, `graphics/actor_part115.c` | default | none: concatenate verbatim |
| M16 | 1 | `src/enemies/enemy_attack.c` | `graphics/actor_part120.c`, `graphics/actor_part122.c` | old_agbcc | none: concatenate verbatim |
| M17 | 2 | `src/enemies/enemy_ctrl.c` | `graphics/actor_part113.c`, `graphics/actor_part116.c`, `graphics/actor_part124.c`, `graphics/actor_part117.c` | default | prototype SetEnemyAnimMode differs only in pointer types; prototype SetEnemyMotionX differs only in pointer types; prototype SetEnemyMotionY differs only in pointer types |
| M18 | 1 | `src/crates/crate_hit.c` | `system/game_loop42.c`, `system/game_loop6.c` | old_agbcc | none: concatenate verbatim |
| M19 | 2 | `src/crates/crate_break.c` | `system/game_loop47.c`, `system/game_loop7.c`, `system/game_loop48.c`, `system/game_loop49.c`, `system/game_loop32.c` | old_agbcc | prototype BreakCrateInStack differs only in pointer types; prototype ClearCrateStackTouched differs only in pointer types; prototype GetCrateAbove differs only in pointer types; prototype GetCrateBelow differs only in pointer types; prototype LightTntCrate differs only in pointer types; prototype MarkCrateStackTouched differs only in pointer types; prototype OpenCheckpointCrate differs only in pointer types |
| M20 | 1 | `src/crates/crate_reset.c` | `system/game_loop33.c`, `system/game_loop22.c` | default | none: concatenate verbatim |
| M21 | 1 | `src/crates/crate.c` | `system/game_loop23.c`, `system/game_loop31.c` | default | none: concatenate verbatim |
| M22 | 2 | `src/crates/crate_stack.c` | `system/game_loop34.c`, `system/game_loop25.c`, `system/game_loop30.c` | default | inline-asm `ldr rN, =sym` in game_loop34.c relies on the end-of-file literal pool: add `asm(".pool")` at its old end |
| M23 | 1 | `src/crates/slot_crate.c` | `system/game_loop26.c`, `system/game_loop27.c` | default | none: concatenate verbatim |
| M24 | 1 | `src/objects/collision_queue.c` | `system/game_loop28.c`, `system/game_loop50.c` | default | none: concatenate verbatim |
| M25 | 2 | `src/pickups/extra_life.c` | `system/game_loop54.c`, `system/game_loop52.c` | old_agbcc | prototype UpdateExtraLifeHop differs only in pointer types |
| M26 | 2 | `src/player/action_ctrl_states.c` | `graphics/actor_part_134b8.c`, `graphics/actor_part_138e8.c`, `graphics/actor_part_13c60.c`, `graphics/actor_part18.c` | old_agbcc | prototype ActionCtrlStateCrawl differs only in pointer types; prototype CheckActionCtrlLeftGround differs only in pointer types; prototype PlayerHasRoomForAnim differs only in pointer types; prototype StartActionCtrlHighJump differs only in pointer types; prototype UpdatePlayerFacing differs only in pointer types; static ActQueue27 duplicated (identical: delete one copy); static ActTrio27 duplicated (identical: delete one copy) |
| M27 | 1 | `src/player/action_ctrl_hang.c` | `graphics/actor_part_14674.c`, `graphics/actor_part38.c` | old_agbcc | none: concatenate verbatim |
| M28 | 1 | `src/player/action_ctrl_moves.c` | `graphics/actor_part38b.c`, `graphics/actor_part38c.c` | old_agbcc | none: concatenate verbatim |
| M29 | 1 | `src/player/action_ctrl.c` | `graphics/actor_part38d.c`, `graphics/actor_part57.c` | default | none: concatenate verbatim |
| M30 | 1 | `src/player/swim_ctrl_stroke.c` | `graphics/actor_part86.c`, `graphics/actor_part86b.c` | old_agbcc | none: concatenate verbatim |
| M31 | 1 | `src/menus/level_select_widgets.c` | `graphics/actor_part_1da38.c`, `graphics/actor_part_1dfec.c` | old_agbcc | none: concatenate verbatim |
| M32 | 1 | `src/gfx/graphics_package.c` | `graphics/graphics_package_1e578.c`, `graphics/graphics_package_1e640.c`, `graphics/graphics_package_1e688.c`, `graphics/graphics_package_1e8f8.c`, `graphics/graphics_package_1e964.c` | old_agbcc | none: concatenate verbatim |
| M33 | 1 | `src/level/spawn_enemies.c` | `graphics/graphics_loading_1ef0c.c`, `graphics/graphics_loading_1fdec.c`, `graphics/graphics_loading_1feec.c` | old_agbcc | none: concatenate verbatim |
| M34 | 1 | `src/level/level_state.c` | `system/game_loop2.c`, `system/game_loop10.c`, `system/game_loop11.c` | default | none: concatenate verbatim |
| M35 | 2 | `src/level/level_query.c` | `system/game_loop17.c`, `system/game_loop18.c` | default | tag struct MedalItemList duplicated (identical: delete one copy); tag struct MedalListItem duplicated (identical: delete one copy); tag struct MedalTableEntry clashes (different body: rename one) |
| M36 | 1 | `src/cutscene/slideshow_display.c` | `system/game_loop19.c`, `system/game_loop38.c`, `system/game_loop20.c` | default | none: concatenate verbatim |
| M37 | 1 | `src/level/entity_flags.c` | `system/game_loop12.c`, `system/game_loop13.c` | default | none: concatenate verbatim |
| M38 | 2 | `src/level/bg_layer.c` | `system/game_loop16.c`, `system/bg_scroll_layer_25fc8.c` | old_agbcc | static Mod32 duplicated (identical: delete one copy) |
| M39 | 1 | `src/level/terrain.c` | `system/game_loop44.c`, `system/game_loop45.c` | default | none: concatenate verbatim |
| M40 | 1 | `src/hud/hud_slide.c` | `graphics/hud_blink.c`, `graphics/hud_icon_widget.c` | default | none: concatenate verbatim |
| M41 | 1 | `src/text/font.c` | `graphics/hud_icon_widget4.c`, `graphics/hud_icon_widget_8a78.c`, `graphics/hud_icon_widget5.c` | default | none: concatenate verbatim |
| M42 | 2 | `src/gfx/sprite_frame.c` | `graphics/sprite_frame_pool.c`, `graphics/sprite_frame_queue.c` | default | tag struct vram_tile_block duplicated (identical: delete one copy) |
| M43 | 1 | `src/actor/actor_category_stats.c` | `graphics/actor_part100.c`, `graphics/actor_part105.c` | default | none: concatenate verbatim |
| M44 | 1 | `src/actor/cell_anim.c` | `graphics/actor_part95.c`, `graphics/actor_part106.c`, `graphics/actor_part96.c`, `graphics/actor_part90.c`, `graphics/actor_part97.c`, `graphics/actor_part91.c`, `graphics/actor_part98.c` | default | none: concatenate verbatim |
| M45 | 1 | `src/actor/actor_bg.c` | `graphics/actor_part92.c`, `graphics/actor_part99.c`, `graphics/actor_part93.c` | default | none: concatenate verbatim |
| M46 | 1 | `src/actor/actor.c` | `graphics/actor_part50.c`, `graphics/actor_part55.c`, `graphics/actor_part56.c`, `graphics/actor_part51.c`, `graphics/actor_part52.c`, `graphics/actor_part53.c`, `graphics/actor_part54.c` | default | none: concatenate verbatim |
| M47 | 1 | `src/vehicle/polar_pickups.c` | `graphics/actor_part19f.c`, `graphics/actor_part19b.c`, `graphics/actor_part19c.c`, `graphics/actor_part19c2.c`, `graphics/actor_part19g.c` | default | none: concatenate verbatim |
| M48 | 1 | `src/vehicle/polar_crates.c` | `graphics/actor_part19d.c`, `graphics/actor_part19i.c` | default | none: concatenate verbatim |
| M49 | 1 | `src/vehicle/polar_objects.c` | `graphics/actor_part126.c`, `graphics/actor_part62.c` | default | none: concatenate verbatim |
| M50 | 1 | `src/vehicle/yeti.c` | `graphics/actor_part60.c`, `graphics/actor_part76.c`, `graphics/actor_part61.c` | default | none: concatenate verbatim |
| M51 | 1 | `src/vehicle/jetpack_run.c` | `graphics/actor_part43.c`, `graphics/actor_part43b.c` | default | none: concatenate verbatim |
| M52 | 1 | `src/vehicle/jetpack_player.c` | `graphics/actor_part44.c`, `graphics/actor_part44b.c`, `graphics/actor_part45.c` | default | none: concatenate verbatim |
| M53 | 1 | `src/vehicle/jetpack_shot.c` | `graphics/actor_part45b.c`, `graphics/actor_part45c.c`, `graphics/actor_part46.c` | default | none: concatenate verbatim |
| M54 | 1 | `src/vehicle/jetpack_plane.c` | `graphics/actor_part46b.c`, `graphics/actor_part_2fbf0.c` | default | none: concatenate verbatim |
| M55 | 1 | `src/bosses/airship_fireball.c` | `graphics/actor_part20.c`, `graphics/actor_part20b.c`, `graphics/actor_part20d.c`, `graphics/actor_part21.c`, `graphics/actor_part21b.c`, `graphics/actor_part22.c` | default | none: concatenate verbatim |
| M56 | 2 | `src/bosses/airship_states.c` | `graphics/actor_part21c.c`, `graphics/actor_part21d.c`, `graphics/actor_part21e.c` | default | static BossSetState duplicated (identical: delete one copy) |
| M57 | 2 | `src/bosses/airship.c` | `graphics/actor_part23c.c`, `graphics/actor_part23d.c`, `graphics/actor_part23e.c`, `graphics/actor_part23f.c`, `graphics/actor_part24.c` | default | static BossSetState duplicated (identical: delete one copy) |
| M58 | 1 | `src/bosses/airship_graphics.c` | `graphics/actor_part26c.c`, `graphics/actor_part26.c` | default | none: concatenate verbatim |
| M59 | 2 | `src/bosses/hovercraft_cannon.c` | `graphics/actor_part29.c`, `graphics/actor_part30.c`, `graphics/actor_part31.c`, `graphics/actor_part32.c`, `graphics/actor_part33.c`, `graphics/actor_part34.c` | default | prototype GetHovercraftAttack differs only in pointer types; tag struct spawner duplicated (identical: delete one copy) |
| M60 | 2 | `src/bosses/hovercraft_launcher.c` | `graphics/actor_part35.c`, `graphics/actor_part36.c`, `graphics/actor_part37.c`, `graphics/actor_part63.c`, `graphics/actor_part64.c`, `graphics/actor_part65.c` | default | tag struct spawner duplicated (identical: delete one copy) |
| M61 | 2 | `src/bosses/hovercraft_side_gun.c` | `graphics/actor_part66.c`, `graphics/actor_part67.c` | default | prototype GetHovercraftAttack differs only in pointer types |
| M62 | 1 | `src/bosses/hovercraft_cannon_flash.c` | `graphics/actor_part68.c`, `graphics/actor_part69.c`, `graphics/actor_part70.c`, `graphics/actor_part71.c` | default | none: concatenate verbatim |
| M63 | 2 | `src/frontend/starfield.c` | `graphics/actor_part85.c`, `graphics/actor_part72.c`, `graphics/actor_part73.c` | default | tag struct particle_bg duplicated (identical: delete one copy); tag struct particle_slot duplicated (identical: delete one copy) |
| M64 | 2 | `src/menus/continue_prompt.c` | `graphics/actor_part88.c`, `graphics/actor_part89.c` | old_agbcc | tag struct continue_prompt clashes (different body: rename one) |
| M65 | 1 | `src/frontend/title_screen_init.c` | `graphics/level_graphics.c`, `graphics/graphics_loading_35780.c` | old_agbcc | none: concatenate verbatim |
| M66 | 2 | `src/frontend/language_select.c` | `audio/counter_selector.c`, `audio/counter_selector_icons.c` | default | tag struct language_select clashes (different body: rename one) |

### Kept separate (same subject and flags, but blocked)

These neighbours share a subject and identical flags but fail rule 4. They
get separate files with role suffixes. Most blockers are the same handful
of globals declared differently in each file (`gPlayer`, `gLevelState`,
`gOamBuffer`, `gSpriteBankSet`, `gEntityFlags`, `gAirship`, `_call_via_rN`).
Once a cleanup gives each of them one declaration in a header, verified by
`make compare` like any cleanup, most of these pairs become tier-2 merges.
That is a follow-up, not part of the move.

| Files (ROM order) | Same flags? | Why they stay separate |
|---|---|---|
| `graphics/palette_blend.c` / `graphics/fade_screen_mode.c` | yes | extern gBrightnessFade: [struct unk_030007E8 gBrightnessFade] vs [s32 gBrightnessFade] |
| `graphics/settings_menu8a2.c` / `graphics/settings_menu8a3.c` | yes | extern gLinkSession: [struct sio_session *gLinkSession] vs [void *gLinkSession] |
| `graphics/settings_menu3.c` / `graphics/settings_menu4.c` | yes | extern gOamBuffer: [void *gOamBuffer] vs [struct oam_shadow_buffer *gOamBuffer] |
| `graphics/settings_menu21.c` / `graphics/settings_menu18.c` | yes | prototype GetUiText: [void *GetUiText(s32 id)] vs [s32 GetUiText(s32 arg0)]; prototype _call_via_r2: [u32 _call_via_r2(void *arg0, void *arg1, void *arg2)] vs [s32 _call_via_r2(void *arg0, void *arg1, void *arg2)] |
| `graphics/settings_menu9.c` / `graphics/settings_menu11.c` | yes | prototype _call_via_r2: [s32 _call_via_r2(void *arg0, void *arg1, void *arg2)] vs [s32 _call_via_r2(void *arg0, s32 arg1, void *arg2)] |
| `graphics/graphics_73dc.c` / `graphics/graphics_7634.c` | yes | extern gOamBuffer: [void *gOamBuffer] vs [struct oam_buffer *gOamBuffer] |
| `graphics/actor_part11e.c` / `graphics/actor_part11d.c` | yes | extern gPlayer: [struct box_part *gPlayer] vs [struct player *gPlayer] |
| `graphics/actor_part8.c` / `graphics/actor_part9.c` | yes | prototype DestroyMovingSprite: [void DestroyMovingSprite(struct actor *self, u32 arg1)] vs [void DestroyMovingSprite(void *self, s32 flags)]; prototype OperatorNew: [void *OperatorNew(s32 size)] vs [void *OperatorNew(u32 size)]; prototype SetSpritePrevPos: [void SetSpritePrevPos(struct gfx_part *self, s32 x, s32 y)] vs [void SetSpritePrevPos(struct gobj *obj)] |
| `graphics/actor_part47.c` / `graphics/actor_part14.c` | yes | prototype DestroyMovingSprite: [void DestroyMovingSprite(void *self, s32 flags)] vs [void DestroyMovingSprite(struct actor *self, u32 arg1)]; prototype InitMovingSprite: [void InitMovingSprite(void *self)] vs [struct actor *InitMovingSprite(struct actor *part)]; prototype OperatorNew: [void *OperatorNew(u32 size)] vs [void *OperatorNew(s32 size)] |
| `graphics/actor_part15.c` / `graphics/actor_part77.c` | yes | extern gSpriteBankSet: [u8 ***gSpriteBankSet] vs [void ***gSpriteBankSet]; prototype SetSpriteAnimDone: [void SetSpriteAnimDone(void *self, s32 a)] vs [void SetSpriteAnimDone(void *part, u8 val)] |
| `graphics/actor_part77.c` / `graphics/actor_part16.c` | yes | extern gSpriteBankSet: [void ***gSpriteBankSet] vs [u8 ***gSpriteBankSet]; prototype SetSpriteAnimDone: [void SetSpriteAnimDone(void *part, u8 val)] vs [void SetSpriteAnimDone(void *self, s32 a)] |
| `graphics/actor_part109.c` / `system/game_loop42.c` | yes | extern gPlayer: [struct box_part *gPlayer] vs [void *gPlayer] |
| `system/game_loop6.c` / `system/game_loop47.c` | yes | extern gPlayer: [void *gPlayer] vs [struct gobj *gPlayer]; prototype BreakCrateInStack: [void BreakCrateInStack(void *self, u8 arg1, u8 arg2, u8 arg3)] vs [void BreakCrateInStack(struct crate *self, u32 a, u32 b, u32 c)] |
| `system/game_loop31.c` / `system/game_loop24.c` | yes | extern gPlayer: [void *gPlayer] vs [struct gobj *gPlayer]; prototype _call_via_r1: [void *_call_via_r1(void *arg0, void *arg1)] vs [s32 _call_via_r1(void *addr, void *fn)]; prototype _call_via_r1: [void *_call_via_r1(void *arg0, void *arg1)] vs [s32 _call_via_r1(void *self, void *fn)] |
| `system/game_loop24.c` / `system/game_loop34.c` | yes | extern gEntityFlags: [u8 *gEntityFlags] vs [void *gEntityFlags] |
| `system/game_loop30.c` / `system/game_loop26.c` | yes | extern gEntityFlags: [void *gEntityFlags] vs [u8 *gEntityFlags] |
| `graphics/actor_part83.c` / `graphics/actor_part_12fbc.c` | yes | extern gPlayer: [struct act_part *gPlayer] vs [u8 *gPlayer] |
| `graphics/actor_part_12fbc.c` / `graphics/actor_part_134b8.c` | yes | extern gPlayer: [u8 *gPlayer] vs [struct act_part *gPlayer] |
| `graphics/actor_part38.c` / `graphics/actor_part38b.c` | yes | prototype UpdatePlayerFacing: [u8 UpdatePlayerFacing(struct act *self)] vs [void UpdatePlayerFacing(void *self)] |
| `graphics/actor_part_188d0.c` / `graphics/actor_part_1967c.c` | yes | extern gCollidableList: [struct gfx_list *gCollidableList] vs [struct part_list *gCollidableList]; extern gEntityFlags: [void *gEntityFlags] vs [struct level_state *gEntityFlags]; extern gPlayer: [struct gfx_player *gPlayer] vs [struct part *gPlayer]; extern gSpriteBankSet: [u8 ***gSpriteBankSet] vs [void ***gSpriteBankSet]; prototype CreateBossCtrl: [void *CreateBossCtrl(void *self)] vs [void Cre |
| `graphics/actor_part_1cee0.c` / `graphics/actor_part_1da38.c` | yes | prototype LockPalette: [void LockPalette(void *cache, s32 record)] vs [void LockPalette(void *cache, u8 record)]; prototype SetLevelSelectEntryBox: [void SetLevelSelectEntryBox(struct item *it, u32 arg)] vs [void SetLevelSelectEntryBox(struct level_item *self, s32 kind)]; prototype SetLevelSelectEntrySelected: [void SetLevelSelectEntrySelected(struct item *it, s32 arg)] vs [void SetLevelSelectEntr |
| `graphics/graphics_loading_1e990.c` / `graphics/graphics_loading_1ea5c.c` | yes | extern gLevelState: [struct level_state *gLevelState] vs [void *gLevelState] |
| `graphics/graphics_loading_1ea5c.c` / `graphics/graphics_loading_1ef0c.c` | yes | extern gSpriteBankSet: [u8 ***gSpriteBankSet] vs [void ***gSpriteBankSet] |
| `graphics/graphics_loading_1feec.c` / `graphics/trigger_effect.c` | yes | extern gSpriteBankSet: [void ***gSpriteBankSet] vs [u8 ***gSpriteBankSet] |
| `graphics/trigger_effect.c` / `graphics/graphics_loading_21280.c` | yes | extern gLevelState: [struct level_progress *gLevelState] vs [void *gLevelState]; extern gSpriteBankSet: [u8 ***gSpriteBankSet] vs [void ***gSpriteBankSet] |
| `graphics/graphics_loading_21280.c` / `graphics/graphics_loading_21668.c` | yes | extern gLevelState: [void *gLevelState] vs [u8 *gLevelState]; prototype CreatePlatform: [s32 *CreatePlatform(u16 x, u16 y, u16 w, u16 h, s32 id)] vs [s32 CreatePlatform(u16 x, u16 y, u16 w, u16 h, s32 id)] |
| `graphics/graphics_loading_21bfc.c` / `graphics/graphics_loading_21d80.c` | yes | extern gEntityFlags: [struct level_record_table **gEntityFlags] vs [void *gEntityFlags]; prototype SetSpriteAnimDone: [void SetSpriteAnimDone(struct popup_part *part, s32 arg)] vs [void SetSpriteAnimDone(void *part, u8 val)] |
| `system/game_loop56.c` / `system/game_loop8.c` | yes | extern gCamera: [struct gl_scratch *gCamera] vs [void *gCamera]; extern gCrateList: [struct gl_entity_list *gCrateList] vs [void *gCrateList]; extern gLevelLayers: [void *gLevelLayers] vs [u8 *gLevelLayers]; extern gPaletteCache: [void *gPaletteCache] vs [struct palette_cache *gPaletteCache]; extern gPaletteCycles: [u8 *gPaletteCycles] vs [void *gPaletteCycles]; extern gPlayer: [struct gl_player * |
| `system/game_loop29.c` / `system/game_loop14.c` | yes | prototype SetSpriteAnimDone: [void SetSpriteAnimDone(struct spawn_part *part, u8 val)] vs [void SetSpriteAnimDone(struct fx_part *part, s32 val)] |
| `graphics/actor_part54.c` / `graphics/actor_part_2ac28.c` | yes | prototype IsSpawnCollected: [s32 IsSpawnCollected(void *selfArg)] vs [u8 IsSpawnCollected(void *spawn)] |
| `graphics/actor_part107.c` / `graphics/actor_part19.c` | yes | prototype CollectWumpa: [s32 CollectWumpa(void *self)] vs [void CollectWumpa(void *self)] |
| `graphics/actor_part19.c` / `graphics/actor_part19e.c` | yes | extern gPolarPlayerStateFuncs: [u8 gPolarPlayerStateFuncs[]] vs [struct actor_pmf gPolarPlayerStateFuncs[]] |
| `graphics/actor_part62.c` / `graphics/actor_part58.c` | yes | extern gLevelState: [struct game_state *gLevelState] vs [struct level_state *gLevelState]; prototype InitActorPart: [void InitActorPart(void *self, s32 a, s32 b, s32 c, s32 d)] vs [void *InitActorPart(void *self, void *part, s32 b, s32 c, s32 d)] |
| `graphics/actor_part43b.c` / `graphics/actor_part44.c` | yes | extern gLevelState: [void *gLevelState] vs [struct level_state *gLevelState] |
| `graphics/actor_part21e.c` / `graphics/actor_part21f.c` | yes | extern gActorList: [struct actor_self *gActorList] vs [void *gActorList] |
| `graphics/actor_part21f.c` / `graphics/actor_part23.c` | yes | extern gAirship: [struct actor_self *gAirship] vs [void *gAirship] |
| `graphics/actor_part25.c` / `graphics/actor_part26b.c` | yes | extern gAirship: [void *gAirship] vs [struct actor_self *gAirship]; extern gAirshipStateTimer: [s32 gAirshipStateTimer] vs [u32 gAirshipStateTimer] |
| `graphics/actor_part26b.c` / `graphics/actor_part26c.c` | yes | extern gAirshipPalette: [u16 gAirshipPalette[]] vs [u8 gAirshipPalette[]] |
| `graphics/actor_part89.c` / `graphics/actor_part131.c` | yes | extern gAudioContext: [void *gAudioContext] vs [struct AudioContext *gAudioContext]; extern gKeys: [u32 gKeys] vs [struct held_pressed_pair gKeys] |
| `audio/counter_selector_icons.c` / `audio/counter_selector_setup.c` | yes | extern gOamBuffer: [void *gOamBuffer] vs [struct oam_shadow_buffer *gOamBuffer] |

Not proposed for merging at all:

- **Flag boundaries:** all 111. One follow-up is possible: several
  `default` files sandwiched between `old_agbcc` neighbours say in their
  comments that they "match under both compilers". Moving such a file into
  `OLD_AGBCC_OBJS` would let it join its neighbours. That needs the
  per-file evidence the project asks for with any flag change, so it is
  not part of this plan.
- **Data files** (`src/data/`): merging `const` arrays can change their
  alignment and padding, and several are separated by `data/data.s`
  sections. They keep their files.
- **Library files:** already in `lib/` (#573).

## Split candidates (optional follow-up)

Splitting a file at a function boundary is byte-safe for the same reasons a
merge is. These files mix subjects and would read better split. Nothing in
this plan depends on splitting them.

- ~~`gfx/graphics.c` (`graphics.c`): OAM buffer, VRAM DMA queue, OBJ VRAM
  cursor, palette cache and the `Entity` base class.~~ **Done (#767):**
  `gfx/oam_buffer.cpp`, `vram_dma_queue.cpp`, `obj_vram_cursor.cpp`,
  `palette_cache.cpp`, `sprite_bank_set.cpp`, `objects/entity.cpp`, and
  `GetCompletionPercent` in `save/game_progress.cpp`.
- ~~`menus/level_select.c` (`actor_part_1b85c.c`): `CameraLead` and
  `LaunchPad` (objects) ahead of the level-select screen.~~ **Done (#767):**
  `objects/camera_lead.cpp`, `objects/launch_pad.cpp`.
- `level/camera.c` (`camera_follow.c`): `OperatorNew`/`OperatorDelete`
  (C++ runtime) after the camera. **Done** (#770): `system/operator_new.cpp`.
- `bosses/cortex.c` and `bosses/dingodile.c` (`actor_part_188d0.c`,
  `actor_part_1967c.c`): Tiny, Cortex and Dingodile straddle both files.
- `vehicle/jetpack_spawn.c` (`actor_part128.c`): starts with
  `YetiStateStop`.
- `vehicle/jetpack_balloon.c` (`actor_part125.c`): starts with the
  airship's `GetAirshipHpPercent`/`DestroyAirship`.
- `bosses/hovercraft.c` (`actor_part130.c`): starts with the jetpack ring
  and collected wumpa.
- ~~`frontend/credits.c` (`actor_part131.c`): starts with the continue
  prompt's draw/run.~~ **Done (#767):** moved to the end of
  `menus/continue_prompt.cpp`.
- `frontend/language_select.c` (`counter_selector.c`): starts with
  `LoadTaggedAssetBuffered` and the company-logo destructors.
- `actor/actor_anim.c` and `util/aabb_setup.c`: the ROM-tail grab bags
  after libgcc.

## What each move PR has to touch

1. **`git mv` first.** Each file is moved with `git mv` so history and
   `git log --follow` survive. For a merge, `git mv` the first member to
   the target, append the others in ROM order, and delete them. Keep pure
   renames and merges in separate commits, so GitHub shows the renames as
   100%-similar renames.
2. **`ldscript.txt`:** rewrite each `build/crashbandicootxs/src/<old>.o(.text);`
   line to the new path. For a merge, keep the first member's line and
   delete the others. **No line changes position.** The `.rodata` lines of
   `src/data/` don't change.
3. **`Makefile`:** update the paths in `OLD_AGBCC_OBJS` (most entries),
   `NO_STRENGTH_REDUCE_OBJS` (`graphics_loading_35d1c.o` →
   `frontend/title_screen.o`) and `NO_RERUN_LOOP_OPT_OBJS`
   (`link_cable_01db4.o` → `link/link_session_reset.o`). A merged group
   keeps one entry. `O1_OBJS` and `NO_INTERWORK_OBJS` only list `lib/`
   objects and `ARM_OBJS` only `src/iwram/`, so they don't change. Update the file names in
   the comments above the lists too (the strength-reduce comment names
   three title-screen files). `C_SRCS` needs no change. Stale objects under
   `build/` are why each PR verifies from `rm -rf build`.
4. **`tools/report_units.py`:** rewrite the `UNITS` base-object paths
   (`"src/<old>.o"`, about 450 strings). A unit keeps its own address
   range, and units of a merged group all point at the merged object. Many
   files already have several units, so that works today. Leave the
   decomp.dev `category` strings (`actor`, `game_loop`, `overlay_ui`, ...)
   alone in the move PRs, so progress history stays continuous. Remapping
   them to the new directories is a separate decision.
5. **`expected/corrections.txt`:** no entry holds a path, so there is
   nothing functional to change. 11 comment lines name `src/` files and can
   be updated with the rest of the text.
6. **Docs and comments:** about 1,400 path mentions in about 185
   `docs/`/README files, and about 1,300 file-name mentions in about 400
   `src/`/`include/` files (comments like "see actor_part19.c"). Rewrite
   them with a one-off script driven by the TSV:
   - `src/<old>.c` → new path;
   - a bare `<old>.c` → the new file name. Old basenames are unique, new
     basenames are unique, and no new basename is some other file's old
     basename, so a single-pass substitution can't chain;
   - a merged-away file → its merged file.

   `docs/matching/*.md` are historical write-ups, but they are rewritten
   too so their references resolve. The TSV stays in the tree as the
   permanent old→new lookup. `docs/status/*.md` are named after the old
   report categories and are left as they are.
7. **Tools:** `tools/chunk_remaining_work.py` and friends glob
   `src/**/*.c` and need no change. Check `tools/*.py` for hard-coded
   `src/<dir>/` paths; today only comments mention them.

**Verification for every PR:** a full clean `make compare`
(`crashbandicootxs.gba: OK`), then
`rm -rf build objdiff.json && make NON_MATCHING=1 report && objdiff-cli report generate`,
which must show 2056/2059 functions and 100% data. The report needs its
own clean build: 2059/2059 means it reused objects from the normal
(`NON_MATCHING=0`) build, as the trial's report run did.

## Batching order

The move lands in PRs by subsystem, in ROM order where that helps
reviewers. Each PR does its renames and tier-1 merges. Tier-2 merges go in
a final commit of the same PR, or in their own PR if review prefers.

| PR | Directories | Code files in → out | Landed in |
|---|---|---|---|
| 1 | `system/`, `util/`, `text/`, `gfx/`, `audio/`, `link/`, plus the TSV-driven rename/rewrite script | 51 → 40 | #596 |
| 2 | `save/`, `menus/`, `frontend/`, `hud/`, `cutscene/` | 60 → 40 | #597 |
| 3 | `objects/`, `player/`, `enemies/`, `crates/`, `pickups/` | 103 → 71 | #598 |
| 4 | `level/`, `bosses/` | 90 → 56 | #599 |
| 5 | `actor/`, `vehicle/` | 61 → 32 | #600 |

PR 1 also proves the script on the smallest directories. Every later PR is
the same script run on the next directories, plus that batch's merges.
`src/graphics/` disappears once its last file has moved (PR 5). Nothing
depends on the old directory names: headers stay in `include/`, and the
Makefile globs `src/*/*.c`.
