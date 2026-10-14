.section .rodata

@ gCategoryFamily0CellAnim..gCategory0SpawnTable: src/game/vehicles/polar/data/polar_cell_anim.cpp

@ gPolarSpriteSheet: src/game/vehicles/polar/data/polar_sprite_sheet.cpp

@ gCategory1SpawnTable..gCategory2SpawnTable: src/game/vehicles/polar/data/polar_spawn_tables.cpp

@ gPolarPlayerRleFrames..gYetiRleFrames: src/game/vehicles/polar/data/polar_rle_sprites.cpp

@ gCategoryFamily1CellAnim..gCategory3SpawnTable: src/game/vehicles/jetpack/data/jetpack_cell_anim.cpp

@ gJetpackSpriteSheet: src/game/vehicles/jetpack/data/jetpack_sprite_sheet.cpp

@ gCategory4BgPicture..gCategory6SpawnTable: src/game/vehicles/jetpack/data/jetpack_sky_pictures.cpp

@ gJetpackPlayerRleFrames: src/game/vehicles/jetpack/data/jetpack_rle_sprites.cpp

@ gAirshipPalette..gSineTable: src/game/bosses/data/boss_pictures.cpp

@ gSongTable: src/engine/audio/data/song_table.cpp

.section .rodata.0816AA6C

.global gSfxTable
gSfxTable:
	@ GAX2 sound-effect trigger table: 99 entries of {slot_id, pitch_offset,
	@ volume}, indexed by the sound effect IDs passed to PlaySfx. Reuses
	@ the same instrument/sample pool as the music (sound/gax_manifest.json)
	@ instead of storing separate sound-effect audio.
	@
	@ Built from sound/sfx_table.json by tools/sfx_table.py.
	.incbin "build/crashbandicootxs/sound/sfx_table.bin"

@ gCrc16Table..gCrash3LinkText: src/engine/link/data/link_crc_table.cpp

@ gMenuCursorText..gPauseTimeTrialIconPos: src/game/ui/data/menu_tables.cpp

@ gPauseMenuBg: src/game/ui/data/pause_menu_bg.cpp

@ gPauseMenuRows..gPauseMenuPalette: src/game/ui/data/pause_menu_rows.cpp

@ gObjPieceWidths..gEmptySpritePoint: src/engine/gfx/data/obj_piece_sizes.cpp

@ gCtrlMotionRecords..gInputCtrlMotionRecords: src/game/crash/data/motion_records.cpp

@ gActionCtrlMotionSet..gSwimCtrlMotionSet: src/game/crash/data/action_swim_motion_sets.cpp

@ gInputCtrlMotionSet..gInputCtrlMotionEntries: src/game/crash/data/input_ctrl_motion_set.cpp

@ gEnemyDefaultAnimMap..gSaucerLabAssistantAnimMap: src/game/enemies/data/enemy_anim_maps.cpp

@ gEnemyCtrlMotionSet..gWumpaHopWidths: src/game/crates/data/crate_tables.cpp

@ gActionCtrlStateTable..gSwimCtrlModeAnimRows: src/game/crash/data/action_ctrl_state_table.cpp

@ gSwimCtrlTurnSpeeds..gSwimCtrlModeLevelAnims: src/game/crash/data/swim_speeds.cpp

@ gSwimCtrlStateFuncs..gMegaMixMotionSet: src/game/crash/data/swim_input_ctrl_state_tables.cpp

@ gMegaMixMotionRecords..gDingodileStalactiteFallMotion: src/game/bosses/data/boss_motion_tables.cpp

@ gDingodileMotionEntries..gPlatformMoverMotionSet: src/game/bosses/data/dingodile_motion_entries.cpp

@ gPlatformMoverMotionRecords: src/game/objects/data/platform_mover_motion.cpp

@ gMenuSkyBg: src/game/ui/data/menu_sky_bg.cpp

@ gLevelSelectWorldPos..gLevelSelectPalette: src/game/ui/data/level_select_layout.cpp

@ gLevelSelectPageBg: src/game/ui/data/level_select_page_bg_package.cpp

@ gZoomBgSlotOffsets..gObjSizeHeights: src/game/ui/data/level_select_anims.cpp

@ gEntitySpawnFuncs: src/game/level/data/entity_spawn_funcs.cpp

@ gThemePaletteCycle2..gThemeMusicCues: src/game/level/data/level_table.cpp

@ gCutsceneTextEnglish..gCutscene10DutchPage5: src/game/ui/data/cutscenes.cpp, cutscene_text_english.c, cutscene_slides.cpp, cutscene_text_translations.c

@ gTerrainTypes..0x08172CD4 (terrain types): src/game/level/data/terrain_types.cpp

@ 0x08172CD4..0x08174BE0 (the UI text of six languages): src/game/ui/data/ui_text.c

@ gHudPartAnims..gLargeFontGlyphs: src/game/ui/data/hud_font_tables.cpp

@ gActorCategories..gActorCategoryVtables: src/engine/actor/data/actor_categories.cpp

@ gActorPaletteCycleFrames..gActorPaletteCycleTargetFrames: src/engine/actor/data/actor_palette_cycle_frames.cpp

@ gPolarCategoryPalette..0x0817A6B8 (categories 0-2 family data): src/game/vehicles/polar/data/polar_anim_family.cpp

@ gPolarPlayerStateFuncs: src/game/vehicles/polar/data/polar_player_state_table.cpp

@ gPolarPlayerShockPalette..gYetiChargeParams: src/game/vehicles/polar/data/polar_palettes_boxes.cpp

@ gYetiStateFuncs: src/game/vehicles/polar/data/yeti_state_table.cpp

@ gYetiKeyframes: src/game/vehicles/polar/data/yeti_keyframes.cpp

@ gYetiFrames: src/game/vehicles/polar/data/yeti_frames.cpp

@ gYetiPalette..0x0817C1C0 (categories 3-6 family data): src/game/vehicles/jetpack/data/jetpack_anim_family.cpp

@ gJetpackPlayerStateFuncs: src/game/vehicles/jetpack/data/jetpack_player_state_table.cpp

@ gJetpackFlashPalettes: src/game/vehicles/jetpack/data/jetpack_flash_palettes.cpp

@ gJetpackPlaneStateFuncs..gJetpackBomberStateFuncs: src/game/vehicles/jetpack/data/jetpack_plane_bomber_state_tables.cpp

@ gAirshipFireballStateFuncs: src/game/vehicles/jetpack/data/airship_fireball_state_table.cpp

@ gAirshipAttacks..gAirshipKeyframes: src/game/vehicles/jetpack/data/airship_attacks.cpp

@ gAirshipStateFuncs: src/game/vehicles/jetpack/data/airship_state_table.cpp

@ gJetpackBalloonStateFuncs: src/game/vehicles/jetpack/data/jetpack_balloon_state_table.cpp

@ gJetpackBalloonCrateStateFuncs: src/game/vehicles/jetpack/data/jetpack_balloon_crate_state_table.cpp

@ gJetpackRocketBox: src/game/vehicles/jetpack/data/jetpack_rocket_box.cpp

@ gHovercraftFireballStateFuncs: src/game/bosses/data/hovercraft_fireball_state_table.cpp

@ gHovercraftAttacks..gHovercraftKeyframes: src/game/bosses/data/hovercraft_attacks.cpp

@ gHovercraftStateFuncs..gHovercraftLauncherStateFuncs: src/game/bosses/data/hovercraft_state_tables.cpp

@ gContinuePromptCursorText..gContinuePromptPalette3: src/game/ui/data/continue_prompt_palettes.cpp

@ gContinuePromptSmokeBg..gContinuePromptUkaUkaBg: src/game/ui/data/continue_prompt_bgs.cpp

@ gCreditsText..gCreditsEmptyText: src/game/ui/data/credits_text.cpp

@ gCreditsLogos..gTitleLogoPieceSeeds: src/game/ui/data/credits_title_logos.cpp

@ gTitleArrowPieceOffsets..gLogoActorAnim: src/game/ui/data/title_screen_gfx.cpp

@ gVvLogoPieceSeeds..gVvLogoUrlObj: src/game/ui/data/vv_logo.cpp

@ gUniversalLogoBg..0x0817E714: src/game/ui/data/universal_vv_logo_motions.cpp

@ gLanguageNames: src/game/ui/data/language_names.cpp

@ gLanguageSelectPalette0..gLanguageSelectPalette2: src/game/ui/data/language_select_palettes.cpp

@ gLanguageSelectPalette3..gUnderwaterBg0Tiles: src/game/level/data/level_tilesets_jungle_snow_underwater.cpp

@ 0x0824B638..0x08270F08 (33 rooms' level data): src/game/level/data/level_rooms_main_bonus.cpp
@ gSewerBg0Tiles..gSpaceBg0Tiles: src/game/level/data/level_tilesets_sewer_space.cpp
@ 0x082B91D0..0x082BF120 (8 rooms' level data): src/game/level/data/level_rooms_bosses_gem_paths.cpp
@ gSpriteBank00Tiles..gObjPalettes: src/engine/gfx/data/sprite_tiles.cpp

@ gSpriteBankTable..gSpriteBank55: src/engine/gfx/data/sprite_banks_00_09.cpp,
@ sprite_banks_10_21.cpp, sprite_banks_22_38.cpp, sprite_banks_39_55.cpp

.section .rodata.084C0006

@ The two GAX2 blocks are tools/gax_audio.py's assembly: their bytes, with
@ every pointer (into a block, or to an engine function) a relocation, so
@ they can move and change size (#967, docs/shifting.md). Each starts
@ word-aligned (in the ROM, the linker's 2 zero bytes after the sprite
@ banks, 0x084C0006-0x084C0008).
	.balign 4, 0
.global gGaxSfxData
gGaxSfxData:
	@ Shin'en GAX2 sound-effect data set: 88 instruments, 87 8-bit
	@ samples, the sample table, and the one handler type every
	@ sound-effect voice uses (its song data points at the instrument and
	@ sample tables). PlaySfx's instrument ids index it.
	@
	@ Built from sound/gax_sfx_manifest.json and sound/sfx_samples/*.wav by
	@ tools/gax_audio.py --sfx; see docs/audio.md.
	.include "build/crashbandicootxs/sound/gax_sfx_data.s"

	.balign 4, 0
.global gGaxMusicData
gGaxMusicData:
	@ Starts with GaxSongHeader.sfxTypes (StartSong): 9 pointers to the
	@ sound-effect voice type above. Then the music data: instrument/sample pool plus all
	@ 19 songs (jungle, underwater, arctic, sewers, future, rocket crash,
	@ bonus round, dingodile, n gin, tiny, neo cortex, main menu europe,
	@ main menu japan, cutscenes, cutscenes spooky, intro, warp room,
	@ credits, drums). Music by Manfred Linzner.
	@
	@ Built from editable sources (sound/songs/*.xm, sound/samples/*.wav,
	@ sound/gax_manifest.json) by tools/gax_audio.py - a from-scratch GAX2
	@ encoder, reverse-engineered to reproduce Shin'en's own object layout
	@ (allocation order, pattern/instrument sharing, even a couple of fixed
	@ "reserved" pointers whose purpose isn't otherwise understood). With
	@ unedited sources this rebuilds byte-for-byte identical to the
	@ original ROM; editing a .xm/.wav changes only the bytes that
	@ actually need to differ.
	.include "build/crashbandicootxs/sound/gax_audio_data.s"

.global gGaxDefaultSong
gGaxDefaultSong:
	@ The engine's default handler layout (GAX2_init, GAX2_estimate): the
	@ song struct of a silent one-channel song whose other objects end the
	@ music block. Built with it by tools/gax_audio.py (manifest
	@ `default_song`); see docs/audio.md. Its pointers are relocations
	@ against gGaxMusicData, so it must follow the block directly.
	.include "build/crashbandicootxs/sound/gax_default_layout.s"

@ __clz_tab_divdi3..__clz_tab_udivdi3: lib/libgcc/data/clz_tab.c

@ gSmallFontTiles..gLargeFontTiles: src/engine/text/data/font_tiles.cpp

@ 0x085A6100..gGaxVibratoTable (GAX2 strings and tables): lib/gax/data/gax_tables.c

@ 0x085A9EEC..0x085A9F70 (EEPROM library data): lib/agb_eeprom/data/eeprom_data.c

@ 0x085A9F70..0x0861BADC (the 24 cutscene pictures): src/game/ui/data/cutscene_pictures.cpp

@ gMenuSkyBgPalette..gVvLogoUrlTiles: src/game/ui/data/menu_graphics.cpp

@ gLevelSelectCursorZoomTiles..gLevelSelectPicture6Palette: src/game/ui/data/level_select_graphics.cpp

@ gSnowBgTiles..gUnderwaterBgTiles: src/game/level/data/level_bg_tiles.cpp

@ gRoom00Asset..gRoom33Asset (the 41 level assets): src/game/level/data/level_assets.cpp

@ gEntityVtable..gLogoActorVtable: the C++ classes' vtables, each object's
@ .gnu.linkonce.d._vt.* section placed by ldscript.txt

@ 0x087E55E4..0x087E5FCC: the IWRAM image (asm/intr_main.s, src/engine/*/*_arm.cpp,
@ src/engine/sys/iwram_data.cpp),
@ linked to run at 0x03000000 - the `iwram` section in ldscript.txt.
@ 0x087E5FCC..0x08800000: 0xFF cartridge fill, the `rom_fill` section.

