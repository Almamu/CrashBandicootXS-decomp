# File layout (#1071)

The source tree follows the namespaces of the code (#1033,
[cplusplus.md](cplusplus.md) "Namespaces"), and no file name carries a
ROM address. [CONTRIBUTING.md](../CONTRIBUTING.md) "Source layout" has
the layout and the naming rules; this page records how the tree got
there: the decisions and the map from every old path to its new one.

The ROM didn't change: the object order is `ldscript.txt`'s, and only the
paths in it moved. Each step was checked with a clean `make compare`, and
the .map compared with the one before, with the old object paths mapped
to the new ones: every address is the same.

## The layout

```
src/engine/{sys,gfx,actor,audio,save,link,text,util}/
src/game/{crash,entities,enemies,bosses,crates,objects,level,ui}/
src/game/vehicles/{polar,jetpack}/
src/<area>/<namespace>/data/        the ROM data tables of that code
include/engine/... include/game/... mirroring src/
include/{core.h,globals.h,match.h,byte_arg.h,screen.h,gba/,constants/}
```

## Decisions

- **A file goes with the namespace of what it defines.** The namespace of
  each object's definitions was read from its compiled `.s` (the `Q`
  names of its global symbols). So the spawners go with the classes they
  create, not with the level that calls them (`spawn_crates.cpp` in
  `game/crates/`, `spawn_enemies.cpp` in `game/enemies/`, ...),
  `display.cpp` and `vram_dma_queue.cpp` are `Engine::Sys`'s, and
  `collision_queue.cpp` is `Game::Crates`'s (CollisionQueue is in that
  namespace). A file with only C-linkage functions or data goes with its
  users: the IWRAM image's ARM routines are split by what they serve
  (`engine/gfx/sprite_arm.cpp`, `engine/util/string_arm.cpp`,
  `engine/sys/iwram_data.cpp`), `asset.cpp` (the tagged-asset loader)
  is graphics, `main_loop.cpp` (`MainLoop`, the game's top-level loop) is
  `Game::Level`'s. A file with classes of both vehicles goes in
  `game/vehicles/` itself.
- **Shared headers stay at the root of `include/`**: `core.h`,
  `globals.h`, `match.h`, `byte_arg.h` (the matching helpers), `screen.h`
  (the screen size, #979), `gba/` and `constants/`. The subsystem headers
  (`gfx.h`, `level.h`, `system.h`, ...) go with their subsystem.
- **Includes name the header's path from `include/`**
  (`#include "engine/gfx/cell_anim.hpp"`): the Makefile's search path is
  unchanged (`-iquote include`, and `-iquote build/include` for the
  generated headers, which keep their names: `graphics.hpp`,
  `rooms.hpp`, `constants/*.h`).
- **The Makefile finds the sources at any depth** (`find src -name
  '*.cpp'`); the object of `src/X.cpp` is still
  `build/crashbandicootxs/src/X.o`, and the per-object lists
  (`OLD_AGBCC_OBJS`, `NO_IMPLEMENT_INLINES_OBJS`, `ARM_OBJS`, ...) name
  the new paths. An existing build keeps working after the move: the
  `.d` files' headers are phony targets (`-MP`), so a header that moved
  away doesn't stop make, and the old objects are simply no longer
  linked (`make tidy` clears them).
- **The report units keep their structure**: `tools/report_units.py`'s
  table names the new object paths, so a unit's name follows its file.
- **Vague names were renamed after their contents** as they moved:
  `inline_copies_misc.cpp` is `aabb_setters.cpp`, `inline_copies_actors.cpp`
  is `actor_destructors.cpp`, `actor.cpp` is `actor_self.cpp`,
  `actor_factory.cpp` is `polar_actor_factory.cpp`, `gfx_part.h` is
  `sprite_piece_flags.h`, `box_part.h` is `sprite_flags.h`, `action_obj.h`
  is `action_input.h` and `crate_line_step.hpp` (generic Bresenham steppers,
  `Engine::Util`) is `line_steppers.hpp`. No file was merged or split.
- **The data tables are named after their content and owner**, in the
  `data/` subdirectory of their owner's directory; a table that serves
  several owners goes with the first one its header comment names. Their
  names are unique, so the Makefile finds a table's object by its name
  (`$(call data_obj,NAME)`; `DATA_OBJS` are all of them, which get the
  generated directories' `-iquote`s), and the names the generators use
  follow the tables': the generated `.inc` files (`ui_text.inc`,
  `level_rooms_main_bonus.inc`, `level_table.inc`, ...), levels.json's
  regions and assets (ROM layout only: the save version's level-data CRC
  doesn't read them) and graphics/manifest.json's objects. A tag or a
  local struct named after the old file (`COMPILE_TIME_ASSERT`'s first
  argument, `struct cell_anim_0ff1b0`) is renamed with it.
- **lib/ keeps its layout**: only its three address-named data files lose
  the address (`eeprom_data.c`, `gax_tables.c`, `clz_tab.c`).
- **The docs' paths were updated**, the matching archive's too, so its
  links point at the new files (a `.c` path of the C era is mapped like
  the `.cpp` it became). A path to a file that was already gone before the
  move (`src/gfx/graphics.c`, ...) keeps its name, as do the frozen
  `expected/` files.

## The rename map

<!-- rename map -->

### The engine sources and headers (moved)


**Engine::Sys**

| Old path | New path |
|---|---|
| `src/system/bios_util.cpp` | `src/engine/sys/bios_util.cpp` |
| `src/system/main.cpp` | `src/engine/sys/main.cpp` |
| `src/system/memory.cpp` | `src/engine/sys/memory.cpp` |
| `src/system/irq.cpp` | `src/engine/sys/irq.cpp` |
| `src/system/key_input.cpp` | `src/engine/sys/key_input.cpp` |
| `src/system/input.cpp` | `src/engine/sys/input.cpp` |
| `src/system/iwram_alloc.cpp` | `src/engine/sys/iwram_alloc.cpp` |
| `src/system/operator_new.cpp` | `src/engine/sys/operator_new.cpp` |
| `src/gfx/display.cpp` | `src/engine/sys/display.cpp` |
| `src/gfx/vram_dma_queue.cpp` | `src/engine/sys/vram_dma_queue.cpp` |
| `src/iwram/iwram_data.cpp` | `src/engine/sys/iwram_data.cpp` |
| `include/system.h` | `include/engine/sys/system.h` |
| `include/memory.h` | `include/engine/sys/memory.h` |
| `include/irq.h` | `include/engine/sys/irq.h` |
| `include/irq.hpp` | `include/engine/sys/irq.hpp` |
| `include/frame_limiter.hpp` | `include/engine/sys/frame_limiter.hpp` |
| `include/key_input.hpp` | `include/engine/sys/key_input.hpp` |
| `include/display.hpp` | `include/engine/sys/display.hpp` |
| `include/vram_dma_queue.hpp` | `include/engine/sys/vram_dma_queue.hpp` |
| `include/iwram.h` | `include/engine/sys/iwram.h` |

**Engine::Gfx**

| Old path | New path |
|---|---|
| `src/system/asset.cpp` | `src/engine/gfx/asset.cpp` |
| `src/gfx/bitmap_screen.cpp` | `src/engine/gfx/bitmap_screen.cpp` |
| `src/gfx/fade.cpp` | `src/engine/gfx/fade.cpp` |
| `src/gfx/fade_to_black.cpp` | `src/engine/gfx/fade_to_black.cpp` |
| `src/gfx/graphics_package.cpp` | `src/engine/gfx/graphics_package.cpp` |
| `src/gfx/oam_buffer.cpp` | `src/engine/gfx/oam_buffer.cpp` |
| `src/gfx/obj_vram_cursor.cpp` | `src/engine/gfx/obj_vram_cursor.cpp` |
| `src/gfx/palette_cache.cpp` | `src/engine/gfx/palette_cache.cpp` |
| `src/gfx/palette_cycle.cpp` | `src/engine/gfx/palette_cycle.cpp` |
| `src/gfx/scaled_sprite.cpp` | `src/engine/gfx/scaled_sprite.cpp` |
| `src/gfx/sprite_bank_set.cpp` | `src/engine/gfx/sprite_bank_set.cpp` |
| `src/gfx/sprite_frame.cpp` | `src/engine/gfx/sprite_frame.cpp` |
| `src/gfx/sprite_renderer.cpp` | `src/engine/gfx/sprite_renderer.cpp` |
| `src/actor/cell_anim.cpp` | `src/engine/gfx/cell_anim.cpp` |
| `src/iwram/sprite_arm.cpp` | `src/engine/gfx/sprite_arm.cpp` |
| `include/gfx.h` | `include/engine/gfx/gfx.h` |
| `include/gfx_part.h` | `include/engine/gfx/sprite_piece_flags.h` |
| `include/cell_anim.hpp` | `include/engine/gfx/cell_anim.hpp` |
| `include/fade.hpp` | `include/engine/gfx/fade.hpp` |
| `include/category_sprite_sheet.hpp` | `include/engine/gfx/category_sprite_sheet.hpp` |
| `include/graphics_package.hpp` | `include/engine/gfx/graphics_package.hpp` |
| `include/obj_tile_allocator.hpp` | `include/engine/gfx/obj_tile_allocator.hpp` |
| `include/sprite_frame_cache.hpp` | `include/engine/gfx/sprite_frame_cache.hpp` |
| `include/sprite_oam_queue.hpp` | `include/engine/gfx/sprite_oam_queue.hpp` |
| `include/sprite_bank.h` | `include/engine/gfx/sprite_bank.h` |
| `include/tagged_asset.h` | `include/engine/gfx/tagged_asset.h` |
| `include/hitbox_quad.h` | `include/engine/gfx/hitbox_quad.h` |

**Engine::Actor**

| Old path | New path |
|---|---|
| `src/actor/actor_bg.cpp` | `src/engine/actor/actor_bg.cpp` |
| `src/actor/actor_category.cpp` | `src/engine/actor/actor_category.cpp` |
| `src/actor/actor_category_frame.cpp` | `src/engine/actor/actor_category_frame.cpp` |
| `src/actor/actor_category_init.cpp` | `src/engine/actor/actor_category_init.cpp` |
| `src/actor/actor_category_select.cpp` | `src/engine/actor/actor_category_select.cpp` |
| `src/actor/actor_category_stats.cpp` | `src/engine/actor/actor_category_stats.cpp` |
| `src/actor/actor_palette_cycle.cpp` | `src/engine/actor/actor_palette_cycle.cpp` |
| `src/actor/actor_spawn.cpp` | `src/engine/actor/actor_spawn.cpp` |
| `src/actor/actor_spawn_collected.cpp` | `src/engine/actor/actor_spawn_collected.cpp` |
| `src/actor/actor_vram_pool.cpp` | `src/engine/actor/actor_vram_pool.cpp` |
| `src/actor/bg_picture.cpp` | `src/engine/actor/bg_picture.cpp` |
| `include/actor.h` | `include/engine/actor/actor.h` |
| `include/actor_anim.h` | `include/engine/actor/actor_anim.h` |
| `include/actor_bg.hpp` | `include/engine/actor/actor_bg.hpp` |
| `include/actor_category.hpp` | `include/engine/actor/actor_category.hpp` |
| `include/actor_category_stats.hpp` | `include/engine/actor/actor_category_stats.hpp` |
| `include/actor_palette_cycle.hpp` | `include/engine/actor/actor_palette_cycle.hpp` |
| `include/actor_spawns.hpp` | `include/engine/actor/actor_spawns.hpp` |
| `include/collected_spawns.hpp` | `include/engine/actor/collected_spawns.hpp` |

**Engine::Audio**

| Old path | New path |
|---|---|
| `src/audio/audio.cpp` | `src/engine/audio/audio.cpp` |
| `include/audio.hpp` | `include/engine/audio/audio.hpp` |

**Engine::Save**

| Old path | New path |
|---|---|
| `src/save/save_data.cpp` | `src/engine/save/save_data.cpp` |
| `src/save/save_transfer.cpp` | `src/engine/save/save_transfer.cpp` |
| `src/save/save_transfer_state.cpp` | `src/engine/save/save_transfer_state.cpp` |
| `include/save.h` | `include/engine/save/save.h` |
| `include/save_data.hpp` | `include/engine/save/save_data.hpp` |

**Engine::Link**

| Old path | New path |
|---|---|
| `src/link/link_handshake.cpp` | `src/engine/link/link_handshake.cpp` |
| `src/link/link_session.cpp` | `src/engine/link/link_session.cpp` |
| `src/link/link_session_reset.cpp` | `src/engine/link/link_session_reset.cpp` |
| `src/link/link_sio.cpp` | `src/engine/link/link_sio.cpp` |
| `include/link.h` | `include/engine/link/link.h` |
| `include/link_session.hpp` | `include/engine/link/link_session.hpp` |

**Engine::Text**

| Old path | New path |
|---|---|
| `src/text/font.cpp` | `src/engine/text/font.cpp` |
| `src/text/font_draw_chars.cpp` | `src/engine/text/font_draw_chars.cpp` |
| `src/text/font_draw_text.cpp` | `src/engine/text/font_draw_text.cpp` |
| `src/text/font_glyph.cpp` | `src/engine/text/font_glyph.cpp` |
| `src/text/font_height.cpp` | `src/engine/text/font_height.cpp` |
| `src/text/text_box.cpp` | `src/engine/text/text_box.cpp` |
| `src/text/wrapped_text.cpp` | `src/engine/text/wrapped_text.cpp` |
| `include/text.h` | `include/engine/text/text.h` |
| `include/font.hpp` | `include/engine/text/font.hpp` |
| `include/bitmap_font.h` | `include/engine/text/bitmap_font.h` |

**Engine::Util**

| Old path | New path |
|---|---|
| `src/util/aabb.cpp` | `src/engine/util/aabb.cpp` |
| `src/util/fixed_math.cpp` | `src/engine/util/fixed_math.cpp` |
| `src/util/line.cpp` | `src/engine/util/line.cpp` |
| `src/util/line_step.cpp` | `src/engine/util/line_step.cpp` |
| `src/util/number_format.cpp` | `src/engine/util/number_format.cpp` |
| `src/util/printf.cpp` | `src/engine/util/printf.cpp` |
| `src/util/rand.cpp` | `src/engine/util/rand.cpp` |
| `src/util/string.cpp` | `src/engine/util/string.cpp` |
| `src/util/time_format.cpp` | `src/engine/util/time_format.cpp` |
| `src/system/inline_copies_misc.cpp` | `src/engine/util/aabb_setters.cpp` |
| `src/iwram/string_arm.cpp` | `src/engine/util/string_arm.cpp` |
| `include/util.h` | `include/engine/util/util.h` |
| `include/math_util.h` | `include/engine/util/math_util.h` |
| `include/aabb.h` | `include/engine/util/aabb.h` |
| `include/line_util.h` | `include/engine/util/line_util.h` |
| `include/crate_line_step.hpp` | `include/engine/util/line_steppers.hpp` |

### The game sources and headers (moved)


**Game::Crash**

| Old path | New path |
|---|---|
| `src/player/action_ctrl.cpp` | `src/game/crash/action_ctrl.cpp` |
| `src/player/action_ctrl_event.cpp` | `src/game/crash/action_ctrl_event.cpp` |
| `src/player/action_ctrl_land.cpp` | `src/game/crash/action_ctrl_land.cpp` |
| `src/player/action_ctrl_moves.cpp` | `src/game/crash/action_ctrl_moves.cpp` |
| `src/player/action_ctrl_states.cpp` | `src/game/crash/action_ctrl_states.cpp` |
| `src/player/action_ctrl_update.cpp` | `src/game/crash/action_ctrl_update.cpp` |
| `src/player/input_ctrl.cpp` | `src/game/crash/input_ctrl.cpp` |
| `src/player/input_ctrl_queue.cpp` | `src/game/crash/input_ctrl_queue.cpp` |
| `src/player/player.cpp` | `src/game/crash/player.cpp` |
| `src/player/player_anim_room.cpp` | `src/game/crash/player_anim_room.cpp` |
| `src/player/player_collide.cpp` | `src/game/crash/player_collide.cpp` |
| `src/player/player_reset.cpp` | `src/game/crash/player_reset.cpp` |
| `src/player/player_update.cpp` | `src/game/crash/player_update.cpp` |
| `src/player/swim_ctrl.cpp` | `src/game/crash/swim_ctrl.cpp` |
| `src/player/swim_ctrl_drift.cpp` | `src/game/crash/swim_ctrl_drift.cpp` |
| `src/player/swim_ctrl_stroke.cpp` | `src/game/crash/swim_ctrl_stroke.cpp` |
| `include/action_ctrl.hpp` | `include/game/crash/action_ctrl.hpp` |
| `include/action_obj.h` | `include/game/crash/action_input.h` |
| `include/input_ctrl.hpp` | `include/game/crash/input_ctrl.hpp` |
| `include/player.hpp` | `include/game/crash/player.hpp` |
| `include/swim_ctrl.hpp` | `include/game/crash/swim_ctrl.hpp` |

**Game::Entities**

| Old path | New path |
|---|---|
| `src/actor/actor.cpp` | `src/game/entities/actor_self.cpp` |
| `src/actor/anim_part.cpp` | `src/game/entities/anim_part.cpp` |
| `src/objects/ctrl.cpp` | `src/game/entities/ctrl.cpp` |
| `src/objects/effect_ctrl.cpp` | `src/game/entities/effect_ctrl.cpp` |
| `src/objects/entity.cpp` | `src/game/entities/entity.cpp` |
| `src/objects/ground_sprite.cpp` | `src/game/entities/ground_sprite.cpp` |
| `src/objects/ground_sprite_collide.cpp` | `src/game/entities/ground_sprite_collide.cpp` |
| `src/objects/moving_sprite.cpp` | `src/game/entities/moving_sprite.cpp` |
| `src/objects/moving_sprite_collide.cpp` | `src/game/entities/moving_sprite_collide.cpp` |
| `src/objects/moving_sprite_probe.cpp` | `src/game/entities/moving_sprite_probe.cpp` |
| `src/objects/part_list.cpp` | `src/game/entities/part_list.cpp` |
| `src/objects/part_list_collide.cpp` | `src/game/entities/part_list_collide.cpp` |
| `src/objects/sprite.cpp` | `src/game/entities/sprite.cpp` |
| `src/objects/sprite_obj.cpp` | `src/game/entities/sprite_obj.cpp` |
| `src/objects/ui_sprite.cpp` | `src/game/entities/ui_sprite.cpp` |
| `include/entity.hpp` | `include/game/entities/entity.hpp` |
| `include/sprite_obj.hpp` | `include/game/entities/sprite_obj.hpp` |
| `include/ctrl.hpp` | `include/game/entities/ctrl.hpp` |
| `include/part_list.hpp` | `include/game/entities/part_list.hpp` |
| `include/actor_self.hpp` | `include/game/entities/actor_self.hpp` |
| `include/objects.h` | `include/game/entities/objects.h` |
| `include/box_part.h` | `include/game/entities/sprite_flags.h` |

**Game::Enemies**

| Old path | New path |
|---|---|
| `src/enemies/enemy_ctrl.cpp` | `src/game/enemies/enemy_ctrl.cpp` |
| `src/enemies/enemy_ctrl_update.cpp` | `src/game/enemies/enemy_ctrl_update.cpp` |
| `src/enemies/enemy_motion.cpp` | `src/game/enemies/enemy_motion.cpp` |
| `src/enemies/enemy_patrol.cpp` | `src/game/enemies/enemy_patrol.cpp` |
| `src/enemies/enemy_shooter.cpp` | `src/game/enemies/enemy_shooter.cpp` |
| `src/enemies/knocked_enemy_ctrl.cpp` | `src/game/enemies/knocked_enemy_ctrl.cpp` |
| `src/enemies/periodic_spawner.cpp` | `src/game/enemies/periodic_spawner.cpp` |
| `src/level/spawn_enemies.cpp` | `src/game/enemies/spawn_enemies.cpp` |
| `include/enemies.h` | `include/game/enemies/enemies.h` |
| `include/enemy_ctrl.hpp` | `include/game/enemies/enemy_ctrl.hpp` |

**Game::Bosses**

| Old path | New path |
|---|---|
| `src/bosses/boss_ctrl.cpp` | `src/game/bosses/boss_ctrl.cpp` |
| `src/bosses/cortex.cpp` | `src/game/bosses/cortex.cpp` |
| `src/bosses/dingodile.cpp` | `src/game/bosses/dingodile.cpp` |
| `src/bosses/dingodile_create.cpp` | `src/game/bosses/dingodile_create.cpp` |
| `src/bosses/hovercraft.cpp` | `src/game/bosses/hovercraft.cpp` |
| `src/bosses/hovercraft_cannon.cpp` | `src/game/bosses/hovercraft_cannon.cpp` |
| `src/bosses/hovercraft_cannon_flash.cpp` | `src/game/bosses/hovercraft_cannon_flash.cpp` |
| `src/bosses/hovercraft_fireball.cpp` | `src/game/bosses/hovercraft_fireball.cpp` |
| `src/bosses/hovercraft_launcher.cpp` | `src/game/bosses/hovercraft_launcher.cpp` |
| `src/bosses/hovercraft_side_gun.cpp` | `src/game/bosses/hovercraft_side_gun.cpp` |
| `src/bosses/hovercraft_state.cpp` | `src/game/bosses/hovercraft_state.cpp` |
| `src/bosses/mega_mix.cpp` | `src/game/bosses/mega_mix.cpp` |
| `src/bosses/mega_mix_update.cpp` | `src/game/bosses/mega_mix_update.cpp` |
| `src/bosses/tiny.cpp` | `src/game/bosses/tiny.cpp` |
| `src/level/spawn_bosses.cpp` | `src/game/bosses/spawn_bosses.cpp` |
| `include/bosses.h` | `include/game/bosses/bosses.h` |
| `include/boss_ctrl.hpp` | `include/game/bosses/boss_ctrl.hpp` |
| `include/boss_actors.hpp` | `include/game/bosses/boss_actors.hpp` |
| `include/hovercraft.hpp` | `include/game/bosses/hovercraft.hpp` |

**Game::Vehicles**

| Old path | New path |
|---|---|
| `src/actor/actor_category_hooks.cpp` | `src/game/vehicles/actor_category_hooks.cpp` |
| `src/actor/inline_copies_actors.cpp` | `src/game/vehicles/actor_destructors.cpp` |
| `include/vehicle.hpp` | `include/game/vehicles/vehicle.hpp` |

**Game::Vehicles::Polar**

| Old path | New path |
|---|---|
| `src/actor/actor_factory.cpp` | `src/game/vehicles/polar/polar_actor_factory.cpp` |
| `src/vehicle/polar/polar_aku_aku.cpp` | `src/game/vehicles/polar/polar_aku_aku.cpp` |
| `src/vehicle/polar/polar_course_objects.cpp` | `src/game/vehicles/polar/polar_course_objects.cpp` |
| `src/vehicle/polar/polar_crate.cpp` | `src/game/vehicles/polar/polar_crate.cpp` |
| `src/vehicle/polar/polar_crates.cpp` | `src/game/vehicles/polar/polar_crates.cpp` |
| `src/vehicle/polar/polar_nitro.cpp` | `src/game/vehicles/polar/polar_nitro.cpp` |
| `src/vehicle/polar/polar_objects.cpp` | `src/game/vehicles/polar/polar_objects.cpp` |
| `src/vehicle/polar/polar_pickups.cpp` | `src/game/vehicles/polar/polar_pickups.cpp` |
| `src/vehicle/polar/polar_player.cpp` | `src/game/vehicles/polar/polar_player.cpp` |
| `src/vehicle/polar/polar_player_states.cpp` | `src/game/vehicles/polar/polar_player_states.cpp` |
| `src/vehicle/polar/yeti.cpp` | `src/game/vehicles/polar/yeti.cpp` |
| `src/vehicle/polar/yeti_graphics.cpp` | `src/game/vehicles/polar/yeti_graphics.cpp` |
| `src/vehicle/polar/yeti_states.cpp` | `src/game/vehicles/polar/yeti_states.cpp` |
| `src/vehicle/polar/yeti_update.cpp` | `src/game/vehicles/polar/yeti_update.cpp` |
| `include/yeti.hpp` | `include/game/vehicles/polar/yeti.hpp` |
| `include/polar_crate_ctors.hpp` | `include/game/vehicles/polar/polar_crate_ctors.hpp` |

**Game::Vehicles::Jetpack**

| Old path | New path |
|---|---|
| `src/vehicle/jetpack/airship.cpp` | `src/game/vehicles/jetpack/airship.cpp` |
| `src/vehicle/jetpack/airship_fireball.cpp` | `src/game/vehicles/jetpack/airship_fireball.cpp` |
| `src/vehicle/jetpack/airship_graphics.cpp` | `src/game/vehicles/jetpack/airship_graphics.cpp` |
| `src/vehicle/jetpack/airship_map.cpp` | `src/game/vehicles/jetpack/airship_map.cpp` |
| `src/vehicle/jetpack/airship_states.cpp` | `src/game/vehicles/jetpack/airship_states.cpp` |
| `src/vehicle/jetpack/airship_touch.cpp` | `src/game/vehicles/jetpack/airship_touch.cpp` |
| `src/vehicle/jetpack/jetpack_balloon.cpp` | `src/game/vehicles/jetpack/jetpack_balloon.cpp` |
| `src/vehicle/jetpack/jetpack_bomber.cpp` | `src/game/vehicles/jetpack/jetpack_bomber.cpp` |
| `src/vehicle/jetpack/jetpack_cannonball.cpp` | `src/game/vehicles/jetpack/jetpack_cannonball.cpp` |
| `src/vehicle/jetpack/jetpack_collected_wumpa.cpp` | `src/game/vehicles/jetpack/jetpack_collected_wumpa.cpp` |
| `src/vehicle/jetpack/jetpack_crates.cpp` | `src/game/vehicles/jetpack/jetpack_crates.cpp` |
| `src/vehicle/jetpack/jetpack_plane.cpp` | `src/game/vehicles/jetpack/jetpack_plane.cpp` |
| `src/vehicle/jetpack/jetpack_player.cpp` | `src/game/vehicles/jetpack/jetpack_player.cpp` |
| `src/vehicle/jetpack/jetpack_player_update.cpp` | `src/game/vehicles/jetpack/jetpack_player_update.cpp` |
| `src/vehicle/jetpack/jetpack_rocket.cpp` | `src/game/vehicles/jetpack/jetpack_rocket.cpp` |
| `src/vehicle/jetpack/jetpack_shot.cpp` | `src/game/vehicles/jetpack/jetpack_shot.cpp` |
| `src/vehicle/jetpack/jetpack_spawn.cpp` | `src/game/vehicles/jetpack/jetpack_spawn.cpp` |
| `include/airship.hpp` | `include/game/vehicles/jetpack/airship.hpp` |

**Game::Crates**

| Old path | New path |
|---|---|
| `src/crates/crate.cpp` | `src/game/crates/crate.cpp` |
| `src/crates/crate_break.cpp` | `src/game/crates/crate_break.cpp` |
| `src/crates/crate_create.cpp` | `src/game/crates/crate_create.cpp` |
| `src/crates/crate_fields.cpp` | `src/game/crates/crate_fields.cpp` |
| `src/crates/crate_hit.cpp` | `src/game/crates/crate_hit.cpp` |
| `src/crates/crate_list.cpp` | `src/game/crates/crate_list.cpp` |
| `src/crates/crate_list_collide.cpp` | `src/game/crates/crate_list_collide.cpp` |
| `src/crates/crate_list_draw.cpp` | `src/game/crates/crate_list_draw.cpp` |
| `src/crates/crate_list_reset.cpp` | `src/game/crates/crate_list_reset.cpp` |
| `src/crates/crate_list_update.cpp` | `src/game/crates/crate_list_update.cpp` |
| `src/crates/crate_reset.cpp` | `src/game/crates/crate_reset.cpp` |
| `src/crates/crate_states.cpp` | `src/game/crates/crate_states.cpp` |
| `src/crates/crate_switches.cpp` | `src/game/crates/crate_switches.cpp` |
| `src/crates/crate_time_trial.cpp` | `src/game/crates/crate_time_trial.cpp` |
| `src/crates/crate_update.cpp` | `src/game/crates/crate_update.cpp` |
| `src/objects/collision_queue.cpp` | `src/game/crates/collision_queue.cpp` |
| `src/level/spawn_crates.cpp` | `src/game/crates/spawn_crates.cpp` |
| `include/crates.h` | `include/game/crates/crates.h` |
| `include/crate.hpp` | `include/game/crates/crate.hpp` |
| `include/crate_list.hpp` | `include/game/crates/crate_list.hpp` |

**Game::Objects**

| Old path | New path |
|---|---|
| `src/objects/camera_lead.cpp` | `src/game/objects/camera_lead.cpp` |
| `src/objects/launch_pad.cpp` | `src/game/objects/launch_pad.cpp` |
| `src/objects/platform.cpp` | `src/game/objects/platform.cpp` |
| `src/objects/platform_collide.cpp` | `src/game/objects/platform_collide.cpp` |
| `src/objects/platform_contact.cpp` | `src/game/objects/platform_contact.cpp` |
| `src/objects/platform_create.cpp` | `src/game/objects/platform_create.cpp` |
| `src/objects/platform_mover.cpp` | `src/game/objects/platform_mover.cpp` |
| `src/pickups/extra_life.cpp` | `src/game/objects/extra_life.cpp` |
| `src/pickups/stopwatch.cpp` | `src/game/objects/stopwatch.cpp` |
| `src/pickups/wumpa.cpp` | `src/game/objects/wumpa.cpp` |
| `src/pickups/wumpa_update.cpp` | `src/game/objects/wumpa_update.cpp` |
| `src/level/spawn_gem_platforms.cpp` | `src/game/objects/spawn_gem_platforms.cpp` |
| `src/level/spawn_gems.cpp` | `src/game/objects/spawn_gems.cpp` |
| `src/level/spawn_objects.cpp` | `src/game/objects/spawn_objects.cpp` |
| `src/level/spawn_pickups.cpp` | `src/game/objects/spawn_pickups.cpp` |
| `include/pickups.hpp` | `include/game/objects/pickups.hpp` |
| `include/platform.hpp` | `include/game/objects/platform.hpp` |

**Game::Level**

| Old path | New path |
|---|---|
| `src/system/main_loop.cpp` | `src/game/level/main_loop.cpp` |
| `src/save/game_progress.cpp` | `src/game/level/game_progress.cpp` |
| `src/level/bg_layer.cpp` | `src/game/level/bg_layer.cpp` |
| `src/level/bg_layer_base.cpp` | `src/game/level/bg_layer_base.cpp` |
| `src/level/bg_streamer.cpp` | `src/game/level/bg_streamer.cpp` |
| `src/level/bonus_round.cpp` | `src/game/level/bonus_round.cpp` |
| `src/level/camera.cpp` | `src/game/level/camera.cpp` |
| `src/level/entity_bitmap.cpp` | `src/game/level/entity_bitmap.cpp` |
| `src/level/entity_flags.cpp` | `src/game/level/entity_flags.cpp` |
| `src/level/entity_spawner.cpp` | `src/game/level/entity_spawner.cpp` |
| `src/level/game_frame.cpp` | `src/game/level/game_frame.cpp` |
| `src/level/level_cutscene.cpp` | `src/game/level/level_cutscene.cpp` |
| `src/level/level_layers.cpp` | `src/game/level/level_layers.cpp` |
| `src/level/level_query.cpp` | `src/game/level/level_query.cpp` |
| `src/level/level_state.cpp` | `src/game/level/level_state.cpp` |
| `src/level/play_room.cpp` | `src/game/level/play_room.cpp` |
| `src/level/pooled_bg_layer.cpp` | `src/game/level/pooled_bg_layer.cpp` |
| `src/level/room.cpp` | `src/game/level/room.cpp` |
| `src/level/room_entities.cpp` | `src/game/level/room_entities.cpp` |
| `src/level/room_select.cpp` | `src/game/level/room_select.cpp` |
| `src/level/spawn_markers.cpp` | `src/game/level/spawn_markers.cpp` |
| `src/level/spawn_start_marker.cpp` | `src/game/level/spawn_start_marker.cpp` |
| `src/level/terrain.cpp` | `src/game/level/terrain.cpp` |
| `src/level/terrain_probe.cpp` | `src/game/level/terrain_probe.cpp` |
| `src/level/terrain_probe_axes.cpp` | `src/game/level/terrain_probe_axes.cpp` |
| `src/level/tile_cache.cpp` | `src/game/level/tile_cache.cpp` |
| `src/level/tile_cache_cell.cpp` | `src/game/level/tile_cache_cell.cpp` |
| `src/level/tile_slot_pool.cpp` | `src/game/level/tile_slot_pool.cpp` |
| `include/level.h` | `include/game/level/level.h` |
| `include/level_data.h` | `include/game/level/level_data.h` |
| `include/level_state.hpp` | `include/game/level/level_state.hpp` |
| `include/bg_layer.hpp` | `include/game/level/bg_layer.hpp` |
| `include/bg_scroll_layer.h` | `include/game/level/bg_scroll_layer.h` |
| `include/camera.hpp` | `include/game/level/camera.hpp` |
| `include/entity_flags.hpp` | `include/game/level/entity_flags.hpp` |
| `include/entity_bits.h` | `include/game/level/entity_bits.h` |
| `include/game_progress.hpp` | `include/game/level/game_progress.hpp` |
| `include/spawners.hpp` | `include/game/level/spawners.hpp` |

**Game::Ui**

| Old path | New path |
|---|---|
| `src/hud/hud.cpp` | `src/game/ui/hud.cpp` |
| `src/hud/hud_counters.cpp` | `src/game/ui/hud_counters.cpp` |
| `src/hud/hud_init.cpp` | `src/game/ui/hud_init.cpp` |
| `src/hud/hud_part.cpp` | `src/game/ui/hud_part.cpp` |
| `src/hud/hud_slide.cpp` | `src/game/ui/hud_slide.cpp` |
| `src/menus/continue_prompt.cpp` | `src/game/ui/continue_prompt.cpp` |
| `src/menus/level_select.cpp` | `src/game/ui/level_select.cpp` |
| `src/menus/level_select_cursor.cpp` | `src/game/ui/level_select_cursor.cpp` |
| `src/menus/level_select_entry.cpp` | `src/game/ui/level_select_entry.cpp` |
| `src/menus/level_select_page_bg.cpp` | `src/game/ui/level_select_page_bg.cpp` |
| `src/menus/level_select_pages.cpp` | `src/game/ui/level_select_pages.cpp` |
| `src/menus/pause_menu.cpp` | `src/game/ui/pause_menu.cpp` |
| `src/menus/pause_menu_collectibles.cpp` | `src/game/ui/pause_menu_collectibles.cpp` |
| `src/menus/pause_menu_draw.cpp` | `src/game/ui/pause_menu_draw.cpp` |
| `src/menus/pause_menu_info.cpp` | `src/game/ui/pause_menu_info.cpp` |
| `src/menus/pause_menu_pages_draw.cpp` | `src/game/ui/pause_menu_pages_draw.cpp` |
| `src/menus/pause_menu_pages_init.cpp` | `src/game/ui/pause_menu_pages_init.cpp` |
| `src/menus/pause_menu_widgets.cpp` | `src/game/ui/pause_menu_widgets.cpp` |
| `src/menus/power_dialog.cpp` | `src/game/ui/power_dialog.cpp` |
| `src/menus/power_dialog_draw.cpp` | `src/game/ui/power_dialog_draw.cpp` |
| `src/menus/zoom_bg.cpp` | `src/game/ui/zoom_bg.cpp` |
| `src/frontend/company_logos.cpp` | `src/game/ui/company_logos.cpp` |
| `src/frontend/company_logos_ctor.cpp` | `src/game/ui/company_logos_ctor.cpp` |
| `src/frontend/credits.cpp` | `src/game/ui/credits.cpp` |
| `src/frontend/language_select.cpp` | `src/game/ui/language_select.cpp` |
| `src/frontend/language_select_setup.cpp` | `src/game/ui/language_select_setup.cpp` |
| `src/frontend/starfield.cpp` | `src/game/ui/starfield.cpp` |
| `src/frontend/title_screen.cpp` | `src/game/ui/title_screen.cpp` |
| `src/cutscene/cutscene_player.cpp` | `src/game/ui/cutscene_player.cpp` |
| `src/cutscene/slideshow.cpp` | `src/game/ui/slideshow.cpp` |
| `src/cutscene/slideshow_display.cpp` | `src/game/ui/slideshow_display.cpp` |
| `src/save/save_menu.cpp` | `src/game/ui/save_menu.cpp` |
| `src/save/save_menu_draw.cpp` | `src/game/ui/save_menu_draw.cpp` |
| `src/save/save_menu_input.cpp` | `src/game/ui/save_menu_input.cpp` |
| `include/hud.hpp` | `include/game/ui/hud.hpp` |
| `include/menus.hpp` | `include/game/ui/menus.hpp` |
| `include/frontend.hpp` | `include/game/ui/frontend.hpp` |
| `include/level_select.hpp` | `include/game/ui/level_select.hpp` |
| `include/save_menu.hpp` | `include/game/ui/save_menu.hpp` |
| `include/cutscene.hpp` | `include/game/ui/cutscene.hpp` |
| `include/logo_fly_in.h` | `include/game/ui/logo_fly_in.h` |

### The data tables and lib/'s data files (moved)


**Engine data**

| Old path | New path |
|---|---|
| `src/data/actor_category_175558.cpp` | `src/engine/actor/data/actor_categories.cpp` |
| `src/data/palette_cycle_175760.cpp` | `src/engine/actor/data/actor_palette_cycle_frames.cpp` |
| `src/data/obj_sizes_16b2e0.cpp` | `src/engine/gfx/data/obj_piece_sizes.cpp` |
| `src/data/sprite_tiles_2bf120.cpp` | `src/engine/gfx/data/sprite_tiles.cpp` |
| `src/data/sprite_banks_4a5600.cpp` | `src/engine/gfx/data/sprite_banks_00_09.cpp` |
| `src/data/sprite_banks_4b0ae0.cpp` | `src/engine/gfx/data/sprite_banks_10_21.cpp` |
| `src/data/sprite_banks_4b414c.cpp` | `src/engine/gfx/data/sprite_banks_22_38.cpp` |
| `src/data/sprite_banks_4b9d7c.cpp` | `src/engine/gfx/data/sprite_banks_39_55.cpp` |
| `src/data/song_table_16aa20.cpp` | `src/engine/audio/data/song_table.cpp` |
| `src/data/link_crc_16af10.cpp` | `src/engine/link/data/link_crc_table.cpp` |
| `src/data/font_tiles_5a4e70.cpp` | `src/engine/text/data/font_tiles.cpp` |

**Game::Crash data**

| Old path | New path |
|---|---|
| `src/data/action_table_16bf20.cpp` | `src/game/crash/data/action_ctrl_state_table.cpp` |
| `src/data/player_pmf_16c250.cpp` | `src/game/crash/data/swim_input_ctrl_state_tables.cpp` |
| `src/data/entry_set_16b92c.cpp` | `src/game/crash/data/action_swim_motion_sets.cpp` |
| `src/data/entry_set_16b93c.cpp` | `src/game/crash/data/input_ctrl_motion_set.cpp` |
| `src/data/motion_records_16b304.cpp` | `src/game/crash/data/motion_records.cpp` |
| `src/data/speed_table_16c090.cpp` | `src/game/crash/data/swim_speeds.cpp` |

**Game::Enemies data**

| Old path | New path |
|---|---|
| `src/data/popup_tables_16b98c.cpp` | `src/game/enemies/data/enemy_anim_maps.cpp` |

**Game::Bosses data**

| Old path | New path |
|---|---|
| `src/data/actor_tables_16c2d8.cpp` | `src/game/bosses/data/boss_motion_tables.cpp` |
| `src/data/entry_set_16c418.cpp` | `src/game/bosses/data/dingodile_motion_entries.cpp` |
| `src/data/boss_pictures_167ad4.cpp` | `src/game/bosses/data/boss_pictures.cpp` |
| `src/data/singleton_kind_17c460.cpp` | `src/game/bosses/data/hovercraft_attacks.cpp` |
| `src/data/actor_pmf_17c450.cpp` | `src/game/bosses/data/hovercraft_fireball_state_table.cpp` |
| `src/data/actor_state_17c4c8.cpp` | `src/game/bosses/data/hovercraft_state_tables.cpp` |

**Game::Vehicles::Polar data**

| Old path | New path |
|---|---|
| `src/data/cell_anim_03b8b0.cpp` | `src/game/vehicles/polar/data/polar_cell_anim.cpp` |
| `src/data/sprite_sheet_0b2120.cpp` | `src/game/vehicles/polar/data/polar_sprite_sheet.cpp` |
| `src/data/sub_effect_0c0c38.cpp` | `src/game/vehicles/polar/data/polar_spawn_tables.cpp` |
| `src/data/rle_sprites_0c2758.cpp` | `src/game/vehicles/polar/data/polar_rle_sprites.cpp` |
| `src/data/anim_family_178f80.cpp` | `src/game/vehicles/polar/data/polar_anim_family.cpp` |
| `src/data/actor_pmf_17a6b8.cpp` | `src/game/vehicles/polar/data/polar_player_state_table.cpp` |
| `src/data/actor_tables_17a728.cpp` | `src/game/vehicles/polar/data/polar_palettes_boxes.cpp` |
| `src/data/actor_state_fn_17a840.cpp` | `src/game/vehicles/polar/data/yeti_state_table.cpp` |
| `src/data/anim_frames_17a850.cpp` | `src/game/vehicles/polar/data/yeti_keyframes.cpp` |
| `src/data/frame_table_17a880.cpp` | `src/game/vehicles/polar/data/yeti_frames.cpp` |

**Game::Vehicles::Jetpack data**

| Old path | New path |
|---|---|
| `src/data/cell_anim_0ff1b0.cpp` | `src/game/vehicles/jetpack/data/jetpack_cell_anim.cpp` |
| `src/data/sprite_sheet_14174c.cpp` | `src/game/vehicles/jetpack/data/jetpack_sprite_sheet.cpp` |
| `src/data/bg_picture_151ac4.cpp` | `src/game/vehicles/jetpack/data/jetpack_sky_pictures.cpp` |
| `src/data/rle_sprites_15a050.cpp` | `src/game/vehicles/jetpack/data/jetpack_rle_sprites.cpp` |
| `src/data/anim_family_17aa6c.cpp` | `src/game/vehicles/jetpack/data/jetpack_anim_family.cpp` |
| `src/data/actor_pmf_17c1c0.cpp` | `src/game/vehicles/jetpack/data/jetpack_player_state_table.cpp` |
| `src/data/palette_strip_17c200.cpp` | `src/game/vehicles/jetpack/data/jetpack_flash_palettes.cpp` |
| `src/data/actor_pmf_17c260.cpp` | `src/game/vehicles/jetpack/data/jetpack_plane_bomber_state_tables.cpp` |
| `src/data/actor_pmf_17c2b8.cpp` | `src/game/vehicles/jetpack/data/airship_fireball_state_table.cpp` |
| `src/data/weapon_kind_17c2d0.cpp` | `src/game/vehicles/jetpack/data/airship_attacks.cpp` |
| `src/data/actor_state_17c3fc.cpp` | `src/game/vehicles/jetpack/data/airship_state_table.cpp` |
| `src/data/actor_pmf_17c414.cpp` | `src/game/vehicles/jetpack/data/jetpack_balloon_state_table.cpp` |
| `src/data/actor_pmf_17c42c.cpp` | `src/game/vehicles/jetpack/data/jetpack_balloon_crate_state_table.cpp` |
| `src/data/actor_box_17c444.cpp` | `src/game/vehicles/jetpack/data/jetpack_rocket_box.cpp` |

**Game::Crates data**

| Old path | New path |
|---|---|
| `src/data/object_tables_16bb6c.cpp` | `src/game/crates/data/crate_tables.cpp` |

**Game::Objects data**

| Old path | New path |
|---|---|
| `src/data/velocity_16c460.cpp` | `src/game/objects/data/platform_mover_motion.cpp` |

**Game::Level data**

| Old path | New path |
|---|---|
| `src/data/dispatch_table_16c6a4.cpp` | `src/game/level/data/entity_spawn_funcs.cpp` |
| `src/data/level_table_16c814.cpp` | `src/game/level/data/level_table.cpp` |
| `src/data/terrain_1725a8.cpp` | `src/game/level/data/terrain_types.cpp` |
| `src/data/level_tilesets_17e78c.cpp` | `src/game/level/data/level_tilesets_jungle_snow_underwater.cpp` |
| `src/data/level_rooms_24b638.cpp` | `src/game/level/data/level_rooms_main_bonus.cpp` |
| `src/data/level_tilesets_270f08.cpp` | `src/game/level/data/level_tilesets_sewer_space.cpp` |
| `src/data/level_rooms_2b91d0.cpp` | `src/game/level/data/level_rooms_bosses_gem_paths.cpp` |
| `src/data/level_bg_tiles_63d4c0.cpp` | `src/game/level/data/level_bg_tiles.cpp` |
| `src/data/level_assets_64f830.cpp` | `src/game/level/data/level_assets.cpp` |

**Game::Ui data**

| Old path | New path |
|---|---|
| `src/data/menu_tables_16b138.cpp` | `src/game/ui/data/menu_tables.cpp` |
| `src/data/bg_package_16b284.cpp` | `src/game/ui/data/pause_menu_bg.cpp` |
| `src/data/pause_rows_16b298.cpp` | `src/game/ui/data/pause_menu_rows.cpp` |
| `src/data/bg_package_16c484.cpp` | `src/game/ui/data/menu_sky_bg.cpp` |
| `src/data/map_tables_16c498.cpp` | `src/game/ui/data/level_select_layout.cpp` |
| `src/data/bg_package_16c58c.cpp` | `src/game/ui/data/level_select_page_bg_package.cpp` |
| `src/data/image_table_16c5a0.cpp` | `src/game/ui/data/level_select_picture_table.cpp` |
| `src/data/map_tables_16c5f0.cpp` | `src/game/ui/data/level_select_anims.cpp` |
| `src/data/cutscenes_16d1c8.cpp` | `src/game/ui/data/cutscenes.cpp` |
| `src/data/cutscene_text_16d4d8.cpp` | `src/game/ui/data/cutscene_text_english.cpp` |
| `src/data/cutscene_slides_16e064.cpp` | `src/game/ui/data/cutscene_slides.cpp` |
| `src/data/cutscene_text_16e8a4.cpp` | `src/game/ui/data/cutscene_text_translations.cpp` |
| `src/data/ui_text_172cd4.cpp` | `src/game/ui/data/ui_text.cpp` |
| `src/data/hud_fonts_174be0.cpp` | `src/game/ui/data/hud_font_tables.cpp` |
| `src/data/hud_palettes_17c510.cpp` | `src/game/ui/data/continue_prompt_palettes.cpp` |
| `src/data/bg_package_17c594.cpp` | `src/game/ui/data/continue_prompt_bgs.cpp` |
| `src/data/credits_17c5d0.cpp` | `src/game/ui/data/credits_text.cpp` |
| `src/data/popup_glyphs_17cf40.cpp` | `src/game/ui/data/credits_title_logos.cpp` |
| `src/data/level_gfx_17cff4.cpp` | `src/game/ui/data/title_screen_gfx.cpp` |
| `src/data/slot_seeds_17d6c0.cpp` | `src/game/ui/data/vv_logo.cpp` |
| `src/data/countdown_17d7a4.cpp` | `src/game/ui/data/universal_vv_logo_motions.cpp` |
| `src/data/digit_glyphs_17e714.cpp` | `src/game/ui/data/language_names.cpp` |
| `src/data/palettes_17e72c.cpp` | `src/game/ui/data/language_select_palettes.cpp` |
| `src/data/cutscene_pictures_5a9f70.cpp` | `src/game/ui/data/cutscene_pictures.cpp` |
| `src/data/menu_graphics_61badc.cpp` | `src/game/ui/data/menu_graphics.cpp` |
| `src/data/level_select_graphics_6377c0.cpp` | `src/game/ui/data/level_select_graphics.cpp` |

**lib/ data (the address dropped)**

| Old path | New path |
|---|---|
| `lib/agb_eeprom/data/eeprom_5a9eec.c` | `lib/agb_eeprom/data/eeprom_data.c` |
| `lib/gax/data/gax_tables_5a6100.c` | `lib/gax/data/gax_tables.c` |
| `lib/libgcc/data/clz_tab_5a4c70.c` | `lib/libgcc/data/clz_tab.c` |

<!-- end rename map -->
