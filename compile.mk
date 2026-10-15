# The compile rules and the per-object compiler choices, shared by every
# game's build (#1108): the XS part of the Makefile and games/n2/game.mk
# include it after setting the directories (C_SUBDIR, C_BUILDDIR,
# LIB_SUBDIR, LIB_BUILDDIR, ASM_*, DATA_ASM_*) and the object lists
# (CXX_OBJS, LIBGCC2_OBJS, LIB1FUNCS_OBJS) it refers to. A flag set here
# for an object applies in every game that builds it.

# Translation units the original build compiled with the older compiler
# (old_agbcc's line). Its scheduler loads a constant *before* the byte it
# is combined with (`movs rA, #K; ldrb rB, [..]; ands rA, rB`) where the
# current one loads the byte first - see
# docs/matching/archive/issue-24-boss-actor.md. All of them are C++ now,
# built by old_agbcp (see "C++ objects" below), which, like old_agbcc,
# has no -fprologue-bugfix option.
OLD_AGBCC_OBJS := $(C_BUILDDIR)/game/entities/sprite.o \
                  $(C_BUILDDIR)/game/entities/sprite_obj.o \
                  $(C_BUILDDIR)/game/level/game_progress.o \
                  $(C_BUILDDIR)/engine/gfx/oam_buffer.o \
                  $(C_BUILDDIR)/engine/sys/vram_dma_queue.o \
                  $(C_BUILDDIR)/engine/gfx/obj_vram_cursor.o \
                  $(C_BUILDDIR)/engine/gfx/palette_cache.o \
                  $(C_BUILDDIR)/engine/gfx/sprite_bank_set.o \
                  $(C_BUILDDIR)/game/entities/entity.o \
                  $(C_BUILDDIR)/engine/gfx/sprite_renderer.o \
                  $(C_BUILDDIR)/engine/sys/display.o \
                  $(C_BUILDDIR)/engine/gfx/fade.o \
                  $(C_BUILDDIR)/engine/gfx/sprite_frame.o \
                  $(C_BUILDDIR)/engine/text/wrapped_text.o \
                  $(C_BUILDDIR)/engine/actor/actor_category_init.o \
                  $(C_BUILDDIR)/engine/actor/actor_category_frame.o \
                  $(C_BUILDDIR)/game/vehicles/actor_category_hooks.o \
                  $(C_BUILDDIR)/game/entities/actor_self.o \
                  $(C_BUILDDIR)/engine/actor/actor_spawn_collected.o \
                  $(C_BUILDDIR)/engine/actor/actor_palette_cycle.o \
                  $(C_BUILDDIR)/game/entities/anim_part.o \
                  $(C_BUILDDIR)/game/vehicles/actor_destructors.o \
                  $(C_BUILDDIR)/game/enemies/enemy_patrol.o \
                  $(C_BUILDDIR)/game/enemies/enemy_ctrl_update.o \
                  $(C_BUILDDIR)/game/entities/ground_sprite_collide.o \
                  $(C_BUILDDIR)/game/entities/moving_sprite.o \
                  $(C_BUILDDIR)/game/entities/moving_sprite_collide.o \
                  $(C_BUILDDIR)/game/entities/ground_sprite.o \
                  $(C_BUILDDIR)/game/crates/crate_list_update.o \
                  $(C_BUILDDIR)/game/crates/crate_list_collide.o \
                  $(C_BUILDDIR)/game/crates/crate_list.o \
                  $(C_BUILDDIR)/game/entities/ctrl.o \
                  $(C_BUILDDIR)/game/bosses/tiny.o \
                  $(C_BUILDDIR)/game/enemies/enemy_ctrl.o \
                  $(C_BUILDDIR)/game/enemies/periodic_spawner.o \
                  $(C_BUILDDIR)/game/enemies/knocked_enemy_ctrl.o \
                  $(C_BUILDDIR)/game/entities/effect_ctrl.o \
                  $(C_BUILDDIR)/game/vehicles/polar/polar_player.o \
                  $(C_BUILDDIR)/game/vehicles/jetpack/jetpack_spawn.o \
                  $(C_BUILDDIR)/game/vehicles/jetpack/jetpack_player_update.o \
                  $(C_BUILDDIR)/game/vehicles/jetpack/jetpack_collected_wumpa.o \
                  $(C_BUILDDIR)/game/bosses/hovercraft_fireball.o \
                  $(C_BUILDDIR)/game/bosses/hovercraft.o \
                  $(C_BUILDDIR)/game/ui/credits.o \
                  $(C_BUILDDIR)/game/vehicles/polar/polar_nitro.o \
                  $(C_BUILDDIR)/game/vehicles/jetpack/airship_map.o \
                  $(C_BUILDDIR)/game/vehicles/jetpack/airship_touch.o \
                  $(C_BUILDDIR)/game/bosses/mega_mix_update.o \
                  $(C_BUILDDIR)/game/crash/action_ctrl_moves.o \
                  $(C_BUILDDIR)/engine/actor/bg_picture.o \
                  $(C_BUILDDIR)/game/entities/ui_sprite.o \
                  $(C_BUILDDIR)/game/vehicles/polar/yeti_update.o \
                  $(C_BUILDDIR)/game/vehicles/polar/yeti_graphics.o \
                  $(C_BUILDDIR)/game/crash/player_collide.o \
                  $(C_BUILDDIR)/game/crash/player_reset.o \
                  $(C_BUILDDIR)/game/crash/player_update.o \
                  $(C_BUILDDIR)/game/entities/part_list_collide.o \
                  $(C_BUILDDIR)/engine/gfx/palette_cycle.o \
                  $(C_BUILDDIR)/game/ui/hud_part.o \
                  $(C_BUILDDIR)/game/crash/action_ctrl_event.o \
                  $(C_BUILDDIR)/game/crash/action_ctrl_update.o \
                  $(C_BUILDDIR)/game/crash/swim_ctrl_stroke.o \
                  $(C_BUILDDIR)/game/ui/continue_prompt.o \
                  $(C_BUILDDIR)/game/crash/action_ctrl_states.o \
                  $(C_BUILDDIR)/game/crash/swim_ctrl.o \
                  $(C_BUILDDIR)/game/crash/input_ctrl.o \
                  $(C_BUILDDIR)/game/bosses/cortex.o \
                  $(C_BUILDDIR)/game/bosses/dingodile.o \
                  $(C_BUILDDIR)/game/objects/platform_create.o \
                  $(C_BUILDDIR)/game/objects/platform_collide.o \
                  $(C_BUILDDIR)/game/objects/platform_contact.o \
                  $(C_BUILDDIR)/game/objects/platform.o \
                  $(C_BUILDDIR)/game/objects/platform_mover.o \
                  $(C_BUILDDIR)/game/objects/camera_lead.o \
                  $(C_BUILDDIR)/game/objects/launch_pad.o \
                  $(C_BUILDDIR)/game/ui/level_select.o \
                  $(C_BUILDDIR)/game/ui/level_select_pages.o \
                  $(C_BUILDDIR)/game/ui/level_select_page_bg.o \
                  $(C_BUILDDIR)/game/ui/zoom_bg.o \
                  $(C_BUILDDIR)/game/ui/level_select_entry.o \
                  $(C_BUILDDIR)/game/ui/level_select_cursor.o \
                  $(C_BUILDDIR)/game/level/spawn_start_marker.o \
                  $(C_BUILDDIR)/game/objects/spawn_gems.o \
                  $(C_BUILDDIR)/game/enemies/spawn_enemies.o \
                  $(C_BUILDDIR)/game/bosses/spawn_bosses.o \
                  $(C_BUILDDIR)/game/objects/spawn_objects.o \
                  $(C_BUILDDIR)/game/ui/title_screen.o \
                  $(C_BUILDDIR)/game/ui/company_logos.o \
                  $(C_BUILDDIR)/game/ui/language_select_setup.o \
                  $(C_BUILDDIR)/game/ui/starfield.o \
                  $(C_BUILDDIR)/engine/gfx/graphics_package.o \
                  $(C_BUILDDIR)/engine/gfx/scaled_sprite.o \
                  $(C_BUILDDIR)/game/ui/hud_init.o \
                  $(C_BUILDDIR)/engine/text/font_glyph.o \
                  $(C_BUILDDIR)/engine/text/font_draw_text.o \
                  $(C_BUILDDIR)/engine/text/font.o \
                  $(C_BUILDDIR)/game/ui/hud_counters.o \
                  $(C_BUILDDIR)/game/ui/save_menu_draw.o \
                  $(C_BUILDDIR)/engine/save/save_data.o \
                  $(C_BUILDDIR)/engine/save/save_transfer.o \
                  $(C_BUILDDIR)/game/ui/pause_menu_collectibles.o \
                  $(C_BUILDDIR)/game/ui/pause_menu_pages_init.o \
                  $(C_BUILDDIR)/game/ui/power_dialog.o \
                  $(C_BUILDDIR)/game/ui/power_dialog_draw.o \
                  $(C_BUILDDIR)/game/ui/pause_menu.o \
                  $(C_BUILDDIR)/game/objects/spawn_gem_platforms.o \
                  $(C_BUILDDIR)/game/level/entity_spawner.o \
                  $(C_BUILDDIR)/game/level/bg_layer.o \
                  $(C_BUILDDIR)/game/level/pooled_bg_layer.o \
                  $(C_BUILDDIR)/game/level/tile_slot_pool.o \
                  $(C_BUILDDIR)/game/level/bg_layer_base.o \
                  $(C_BUILDDIR)/game/crates/crate_create.o \
                  $(C_BUILDDIR)/game/ui/slideshow.o \
                  $(C_BUILDDIR)/game/level/tile_cache.o \
                  $(C_BUILDDIR)/game/level/entity_flags.o \
                  $(C_BUILDDIR)/game/level/room_entities.o \
                  $(C_BUILDDIR)/game/crates/crate_hit.o \
                  $(C_BUILDDIR)/game/level/terrain_probe_axes.o \
                  $(C_BUILDDIR)/game/crates/crate_break.o \
                  $(C_BUILDDIR)/game/crates/crate_switches.o \
                  $(C_BUILDDIR)/game/crates/crate_states.o \
                  $(C_BUILDDIR)/game/crates/crate_update.o \
                  $(C_BUILDDIR)/game/crates/crate_fields.o \
                  $(C_BUILDDIR)/game/crates/crate.o \
                  $(C_BUILDDIR)/game/crates/crate_reset.o \
                  $(C_BUILDDIR)/game/crates/crate_time_trial.o \
                  $(C_BUILDDIR)/game/objects/wumpa_update.o \
                  $(C_BUILDDIR)/game/objects/extra_life.o \
                  $(C_BUILDDIR)/game/objects/wumpa.o \
                  $(C_BUILDDIR)/game/objects/stopwatch.o \
                  $(C_BUILDDIR)/game/level/game_frame.o \
                  $(C_BUILDDIR)/game/ui/cutscene_player.o \
                  $(C_BUILDDIR)/game/level/bg_streamer.o \
                  $(C_BUILDDIR)/game/level/play_room.o \
                  $(C_BUILDDIR)/game/level/level_state.o \
                  $(C_BUILDDIR)/game/level/level_query.o \
                  $(C_BUILDDIR)/game/level/room_select.o \
                  $(C_BUILDDIR)/game/crates/spawn_crates.o \
                  $(C_BUILDDIR)/game/objects/spawn_pickups.o \
                  $(C_BUILDDIR)/game/level/spawn_markers.o \
                  $(C_BUILDDIR)/engine/link/link_handshake.o \
                  $(C_BUILDDIR)/engine/link/link_session_reset.o \
                  $(C_BUILDDIR)/engine/link/link_session.o
# #662 step 3: system/util/audio objects whose workarounds (pins, copy
# asms) aren't needed under old_agbcc. Evidence: each object stays
# byte-identical with plain C - input.o's `keys = mask & pressed`
# (old_agbcc copies `mask` first, as the ROM does), irq.o's DISPSTAT
# updates, key_input.o's UpdateKeys (split from irq.o, #767), aabb.o's
# CommitBlendRegs (BLDY's address derived from BLDCNT's; at the end of
# display.o, already old_agbcc, since #767 - aabb.o and iwram_alloc.o, the
# rest of the old aabb.o, stay old_agbcc), audio.o's VCount DISPSTAT updates
# (audio.cpp since, built by old_agbcp: agbcp gives the same AudioContext
# methods but loads DISPSTAT's byte before the mask).
OLD_AGBCC_OBJS += $(C_BUILDDIR)/engine/sys/input.o \
                  $(C_BUILDDIR)/engine/sys/irq.o \
                  $(C_BUILDDIR)/engine/sys/key_input.o \
                  $(C_BUILDDIR)/engine/util/aabb.o \
                  $(C_BUILDDIR)/engine/sys/iwram_alloc.o \
                  $(C_BUILDDIR)/engine/audio/audio.o
# A game whose original build used the current compiler for some of these
# lists them in NEW_COMPILER_OBJS before including this file (none for XS;
# games/n2/game.mk).
OLD_AGBCC_OBJS := $(filter-out $(NEW_COMPILER_OBJS),$(OLD_AGBCC_OBJS))
$(OLD_AGBCC_OBJS): CC1FLAGS := $(filter-out -fprologue-bugfix,$(CC1FLAGS))

# C++ objects (#664, docs/cplusplus.md). The game is g++ 2.x C++; these
# objects are built from C++ source with the C++ front end of the same
# compiler: agbcp, and old_agbcp for the ones in OLD_AGBCC_OBJS. Both are
# built from notyourav/agbcc's `cp` branch by tools/build_agbccpp.sh,
# which ports agbcc's -fprologue-bugfix and OLD_COMPILER switches to it,
# so the C flags above apply unchanged. The game had no RTTI (every
# vtable's slot 0 is empty) and no exceptions. After assembling,
# cxx_symbols.txt renames the mangled names to the C names the rest of
# the build links against.
CXX1     := tools/agbcc/bin/agbcp
CXX1_OLD := tools/agbcc/bin/old_agbcp
CXX_SYMBOLS := cxx_symbols.txt
$(CXX_OBJS): CC1FLAGS += -fno-rtti -fno-exceptions
$(filter $(OLD_AGBCC_OBJS),$(CXX_OBJS)): CXX1 := $(CXX1_OLD)

# C++ objects built with -fno-implement-inlines. Each is the key-method
# object of a class whose vtable g++ emits (docs/cplusplus.md, "Emitting
# the vtables"): there g++ 2.9 also emits an out-of-line copy of every
# inline method of the class, used or not, and the ROM has none of them
# (the copy constructor-like `DingodileProjectileCtrl(rocket)` in
# dingodile.o, ...). The flag drops those copies and nothing else: each
# object's code is the same with and without it as long as the class
# keeps `#pragma interface`. entity.o (graphics.o until #767) is the one
# key-method object the ROM has the copies in (Entity's inline methods,
# DestroyEntity last).
NO_IMPLEMENT_INLINES_OBJS := $(C_BUILDDIR)/game/entities/actor_self.o \
                             $(C_BUILDDIR)/game/vehicles/actor_category_hooks.o \
                             $(C_BUILDDIR)/engine/actor/actor_palette_cycle.o \
                             $(C_BUILDDIR)/engine/actor/actor_spawn_collected.o \
                             $(C_BUILDDIR)/game/entities/anim_part.o \
                             $(C_BUILDDIR)/game/vehicles/actor_destructors.o \
                             $(C_BUILDDIR)/game/bosses/dingodile.o \
                             $(C_BUILDDIR)/game/crates/crate_update.o \
                             $(C_BUILDDIR)/game/ui/company_logos_ctor.o \
                             $(C_BUILDDIR)/game/ui/language_select.o \
                             $(C_BUILDDIR)/game/ui/level_select.o \
                             $(C_BUILDDIR)/game/objects/camera_lead.o \
                             $(C_BUILDDIR)/game/entities/ground_sprite_collide.o \
                             $(C_BUILDDIR)/game/objects/launch_pad.o \
                             $(C_BUILDDIR)/game/entities/moving_sprite_collide.o \
                             $(C_BUILDDIR)/game/objects/platform_contact.o \
                             $(C_BUILDDIR)/game/entities/sprite.o \
                             $(C_BUILDDIR)/game/objects/extra_life.o \
                             $(C_BUILDDIR)/game/objects/wumpa.o \
                             $(C_BUILDDIR)/game/objects/stopwatch.o \
                             $(C_BUILDDIR)/game/crash/action_ctrl_update.o \
                             $(C_BUILDDIR)/game/crash/input_ctrl.o \
                             $(C_BUILDDIR)/game/crash/player_collide.o \
                             $(C_BUILDDIR)/game/vehicles/jetpack/jetpack_crates.o \
                             $(C_BUILDDIR)/game/vehicles/jetpack/jetpack_rocket.o \
                             $(C_BUILDDIR)/game/vehicles/polar/polar_crate.o \
                             $(C_BUILDDIR)/game/vehicles/polar/polar_pickups.o
$(NO_IMPLEMENT_INLINES_OBJS): CC1FLAGS += -fno-implement-inlines

# Objects built with -fno-rerun-loop-opt (one loop-optimizer pass).
# link_session_reset.cpp holds only ResetLinkSessionState, which is real C: it keeps the
# ROM's up-counting inner copy loop only with this flag (the rerun pass
# reverses it; without the flag the matching C is 147 halfwords off and
# 16 bytes long). The flag changes the matching HandleLinkSerial, which is
# why ResetLinkSessionState was split out of link_handshake.cpp. See
# docs/matching/archive/last-ten-naked-retry.md and
# docs/matching/archive/last-eleven-naked-retry.md.
NO_RERUN_LOOP_OPT_OBJS := $(C_BUILDDIR)/engine/link/link_session_reset.o
$(NO_RERUN_LOOP_OPT_OBJS): CC1FLAGS += -fno-rerun-loop-opt

# Objects built with -fno-cse-skip-blocks (cse doesn't carry its table
# around a skipped `if` block). wumpa_update.cpp: Wumpa::Create's `phase`
# 0 stays a register for its compare and store while `counter` gets a
# fresh 0 only when cse loses the 0 at the frame clamp's join; with
# skip-blocks it shares the 0 (the C hid it with an asm). PickUp and
# Update compile the same either way. SendToHud, which needs skip-blocks
# (its two 1s share r4 around ClampFrame's `if`), StartPayout and UpdateHop
# went to the start of wumpa.cpp, the next object in the ROM (#662 round 3).
NO_CSE_SKIP_BLOCKS_OBJS := $(C_BUILDDIR)/game/objects/wumpa_update.o
$(NO_CSE_SKIP_BLOCKS_OBJS): CC1FLAGS += -fno-cse-skip-blocks

# Objects built with -O1 instead of -O2: the whole of lib/agb_eeprom's
# code. Nintendo's AgbEeprom SDK library (the ROM's "EEPROM_V122",
# 0x0803A968-0x0803AD7C) was compiled at -O1, and all nine of its
# functions are the SDK's plain C (TMC/pokeemerald source shape, no pins
# or volatile tricks), byte-identical at -O1:
# - eeprom_timer.o: EEPROMConfigure, EepromTimerIntr (timer
#   IRQ handler), SetEepromTimerIntr and StartEepromTimer. At -O2 the same C is 47 halfwords off in
#   StartEepromTimer; the old -O2 version needed six register pins and a
#   `vu16 * volatile` global.
# - eeprom_timer_stop.o: StopEepromTimer, DMA3Transfer - 2/43 halfwords off at -O2 (-O2 cross-jumps the
#   duplicated DMA-wait test into the loop).
# - eeprom_read_write.o: EEPROMRead/EEPROMWrite -
#   79/107 halfwords off at -O2.
# - eeprom_verify.o: EEPROMCompare/EEPROMWrite1_check.
# It is current agbcc: old_agbcc -O1 is 2/44/35 off for DMA3Transfer/
# EEPROMRead/EEPROMWrite. Every object matches as a whole with the flag.
# EEPROM_V122 is the ROM's only SDK version tag, and no other compiled
# code was found to be SDK C. The library's data object
# (lib/agb_eeprom/data/eeprom_data.o) keeps the default flags. See
# docs/matching/eeprom-sdk-o1.md.
O1_OBJS := $(LIB_BUILDDIR)/agb_eeprom/src/eeprom_timer.o \
           $(LIB_BUILDDIR)/agb_eeprom/src/eeprom_timer_stop.o \
           $(LIB_BUILDDIR)/agb_eeprom/src/eeprom_read_write.o \
           $(LIB_BUILDDIR)/agb_eeprom/src/eeprom_verify.o
$(O1_OBJS): CC1FLAGS := $(filter-out -O2,$(CC1FLAGS)) -O1

# libgcc2.c's objects (__divdi3/__udivdi3/__muldi3, linked in with the
# GAX2 library) were built without -mthumb-interwork: their functions are
# the only ones in the ROM that return via a combined `pop {r4-r7, pc}`,
# and with the flag dropped they compile from libgcc2.c's own source
# byte-for-byte - see lib/libgcc/libgcc2.c and
# docs/matching/archive/gax-toolchain-retry.md.
NO_INTERWORK_OBJS := $(LIBGCC2_OBJS)
$(NO_INTERWORK_OBJS): CC1FLAGS := $(filter-out -mthumb-interwork,$(CC1FLAGS))

# ARM-state code of the IWRAM image (ldscript.txt's `iwram` section),
# written as C++ like the rest of the game and built with
# agbcp_arm_patched: the ARM-targeting C++ compiler of notyourav/agbcc's
# `cp` branch (its g++_arm tree, agbcc_arm's gcc 2.9-arm-000512 with the
# C++ front end) plus tools/agbcc_patches/agbcc_arm_prologue_return.patch,
# built by tools/build_agbccpp.sh. -fomit-frame-pointer: the ROM's ARM
# functions have no APCS frame (the Thumb agbcc never sets one up, the
# ARM one does by default). The prologue bugfix and -fhex-asm are Thumb
# agbcc-only options. See docs/matching/iwram-image.md and
# docs/cplusplus.md, "The IWRAM ARM code".
#
# The ROM's ARM compiler is a later, unreleased build of agbcc_arm's line
# whose prologue and return code differ in two fixed strings, and whose
# jump pass lacks two of agbcc_arm's rules; the patch adds an opt-in
# option for each. Without the options the patched compiler's output is
# identical to agbcc_arm's (and agbcp_arm's), so the objects' other
# functions are unaffected (checked with and without the options). See
# docs/matching/iwram-image.md, "Seventh pass" and "Ninth step".
# - string_arm.o: strncpy_arm's `n == 0` test branches to the final
#   `bx lr` where stock agbcc_arm makes a conditional `bxeq lr` for any C
#   (-mno-cond-return). The other string functions come out the same
#   with or without it. (itoa_arm, which needed -mleaf-no-lr-save and no
#   scheduling, is assembly now: asm/itoa_arm.s, #662.)
# - sprite_arm.o: LookupSpriteFrameCache's three returns pop into lr
#   (-minterwork-return-lr). HeapSortActorsByKey keeps both of its
#   identical second-loop tests (`cmp r7, #1; ble`), which stock jump2
#   cross-jumps into one for any C because each follows a label
#   (-mstrict-cross-jump). Its other three functions come out the same
#   with or without these, and need scheduling, so it keeps it.
CXX1_ARM := tools/agbcc/bin/agbcp_arm_patched
ARM_OBJS := $(C_BUILDDIR)/engine/util/string_arm.o \
            $(C_BUILDDIR)/engine/gfx/sprite_arm.o
$(ARM_OBJS): CXX1 := $(CXX1_ARM)
$(ARM_OBJS): CC1FLAGS := -mthumb-interwork $(WARNFLAGS) -O2 -fomit-frame-pointer -fno-rtti -fno-exceptions
$(C_BUILDDIR)/engine/util/string_arm.o: CC1FLAGS += -mno-cond-return
$(C_BUILDDIR)/engine/gfx/sprite_arm.o: CC1FLAGS += -minterwork-return-lr -mstrict-cross-jump

# Objects whose "might be used uninitialized" warning is expected and left
# enabled on purpose (#662, owner decision): it is printed on every build,
# but doesn't fail it. The C reads the variable exactly as the ROM does, so
# no `x = x` self-init or empty asm hides it any more. agbcc's gcc 2.9 has
# no -Wno-error=uninitialized, so these objects get -Wno-error (each prints
# that one warning and nothing else; keep it that way). The code is the
# same with and without the silencers that used to be there:
# - gax_voice_steal.o: GAX_fx's `sel` is really unset if there is no SFX
#   voice (the ROM returns whatever its register held).
# - eeprom_verify.o: EEPROMWrite1_check's `result` - a false positive, the
#   loop always runs; `= 0` adds a store to the SDK code (#577).
UNINIT_WARNING_OBJS := $(LIB_BUILDDIR)/gax/src/gax_voice_steal.o \
                       $(LIB_BUILDDIR)/agb_eeprom/src/eeprom_verify.o
$(UNINIT_WARNING_OBJS): CC1FLAGS += -Wno-error

# Appended to every compiled .s before it is assembled (#663). agbcc
# starts each function with `.align 2, 0`, but nothing aligns the end of
# the last one, so when a .text section ends on a halfword boundary `as`
# pads it to the section's 4-byte alignment with a Thumb NOP (`mov r8, r8`,
# 0x46C0). The original build left zeros there. Ending .text with an
# explicit zero-fill `.align 2, 0` reproduces that; it adds nothing to a
# section that already ends aligned.
ZERO_PAD_TEXT := \t.text\n\t.align\t2, 0\n

$(C_BUILDDIR)/%.o : $(C_SUBDIR)/%.c
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $(DEPFLAGS) $< | $(CC1) $(CC1FLAGS) -o $(C_BUILDDIR)/$*.s
	printf '$(ZERO_PAD_TEXT)' >> $(C_BUILDDIR)/$*.s
	$(AS) $(ASFLAGS) -o $@ $(C_BUILDDIR)/$*.s

$(C_BUILDDIR)/%.o : $(C_SUBDIR)/%.cpp $(CXX_SYMBOLS)
	@mkdir -p $(dir $@)
	$(CPP) -x c++ $(CPPFLAGS) $(DEPFLAGS) $< | $(CXX1) -quiet $(CC1FLAGS) -o $(C_BUILDDIR)/$*.s
	printf '$(ZERO_PAD_TEXT)' >> $(C_BUILDDIR)/$*.s
	$(AS) $(ASFLAGS) -o $@ $(C_BUILDDIR)/$*.s
	$(OBJCOPY) --redefine-syms=$(CXX_SYMBOLS) $@

$(LIB_BUILDDIR)/%.o : $(LIB_SUBDIR)/%.c
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $(DEPFLAGS) $< | $(CC1) $(CC1FLAGS) -o $(LIB_BUILDDIR)/$*.s
	printf '$(ZERO_PAD_TEXT)' >> $(LIB_BUILDDIR)/$*.s
	$(AS) $(ASFLAGS) -o $@ $(LIB_BUILDDIR)/$*.s

$(LIBGCC2_OBJS): $(LIB_BUILDDIR)/libgcc/%.o: $(LIB_SUBDIR)/libgcc/libgcc2.c $(LIB_SUBDIR)/libgcc/libgcc2_udivmoddi4.h
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $(DEPFLAGS) -DL$* $< | $(CC1) $(CC1FLAGS) -o $(LIB_BUILDDIR)/libgcc/$*.s
	printf '$(ZERO_PAD_TEXT)' >> $(LIB_BUILDDIR)/libgcc/$*.s
	$(AS) $(ASFLAGS) -o $@ $(LIB_BUILDDIR)/libgcc/$*.s

$(LIB1FUNCS_OBJS): $(LIB_BUILDDIR)/libgcc/%.o: $(LIB_SUBDIR)/libgcc/lib1funcs.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) --defsym L$*=1 -o $@ $<

$(LIB_BUILDDIR)/%.o: $(LIB_SUBDIR)/%.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(ASM_BUILDDIR)/%.o: $(ASM_SUBDIR)/%.s
	$(AS) $(ASFLAGS) -o $@ $<

$(DATA_ASM_BUILDDIR)/%.o: $(DATA_ASM_SUBDIR)/%.s
	$(AS) $(ASFLAGS) -o $@ $<
