.section .rodata

@ gCategoryFamily0CellAnim..gCategory0SpawnTable: src/data/cell_anim_03b8b0.cpp

@ gPolarSpriteSheet: src/data/sprite_sheet_0b2120.cpp

@ gCategory1SpawnTable..gCategory2SpawnTable: src/data/sub_effect_0c0c38.cpp

@ gPolarPlayerRleFrames..gYetiRleFrames: src/data/rle_sprites_0c2758.cpp

@ gCategoryFamily1CellAnim..gCategory3SpawnTable: src/data/cell_anim_0ff1b0.cpp

@ gJetpackSpriteSheet: src/data/sprite_sheet_14174c.cpp

@ gCategory4BgPicture..gCategory6SpawnTable: src/data/bg_picture_151ac4.cpp

@ gJetpackPlayerRleFrames: src/data/rle_sprites_15a050.cpp

@ gAirshipPalette..gSineTable: src/data/boss_pictures_167ad4.cpp

@ gSongTable: src/data/song_table_16aa20.cpp

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

@ gCrc16Table..gCrash3LinkText: src/data/link_crc_16af10.cpp

@ gMenuCursorText..gPauseTimeTrialIconPos: src/data/menu_tables_16b138.cpp

@ gPauseMenuBg: src/data/bg_package_16b284.cpp

@ gPauseMenuRows..gPauseMenuPalette: src/data/pause_rows_16b298.cpp

@ gObjPieceWidths..gEmptySpritePoint: src/data/obj_sizes_16b2e0.cpp

@ gCtrlMotionRecords..gInputCtrlMotionRecords: src/data/motion_records_16b304.cpp

@ gActionCtrlMotionSet..gSwimCtrlMotionSet: src/data/entry_set_16b92c.cpp

@ gInputCtrlMotionSet..gInputCtrlMotionEntries: src/data/entry_set_16b93c.cpp

@ gEnemyDefaultAnimMap..gSaucerLabAssistantAnimMap: src/data/popup_tables_16b98c.cpp

@ gEnemyCtrlMotionSet..gWumpaHopWidths: src/data/object_tables_16bb6c.cpp

@ gActionCtrlStateTable..gSwimCtrlModeAnimRows: src/data/action_table_16bf20.cpp

@ gSwimCtrlTurnSpeeds..gSwimCtrlModeLevelAnims: src/data/speed_table_16c090.cpp

@ gSwimCtrlStateFuncs..gMegaMixMotionSet: src/data/player_pmf_16c250.cpp

@ gMegaMixMotionRecords..gDingodileStalactiteFallMotion: src/data/actor_tables_16c2d8.cpp

@ gDingodileMotionEntries..gPlatformMoverMotionSet: src/data/entry_set_16c418.cpp

@ gPlatformMoverMotionRecords: src/data/velocity_16c460.cpp

@ gMenuSkyBg: src/data/bg_package_16c484.cpp

@ gLevelSelectWorldPos..gLevelSelectPalette: src/data/map_tables_16c498.cpp

@ gLevelSelectPageBg: src/data/bg_package_16c58c.cpp

@ gZoomBgSlotOffsets..gObjSizeHeights: src/data/map_tables_16c5f0.cpp

@ gEntitySpawnFuncs: src/data/dispatch_table_16c6a4.cpp

@ gThemePaletteCycle2..gThemeMusicCues: src/data/level_table_16c814.cpp

@ gCutsceneTextEnglish..gCutscene10DutchPage5: src/data/cutscenes_16d1c8.cpp, cutscene_text_16d4d8.c, cutscene_slides_16e064.cpp, cutscene_text_16e8a4.c

@ gTerrainTypes..0x08172CD4 (terrain types): src/data/terrain_1725a8.cpp

@ 0x08172CD4..0x08174BE0 (the UI text of six languages): src/data/ui_text_172cd4.c

@ gHudPartAnims..gLargeFontGlyphs: src/data/hud_fonts_174be0.cpp

@ gActorCategories..gActorCategoryVtables: src/data/actor_category_175558.cpp

@ gActorPaletteCycleFrames..gActorPaletteCycleTargetFrames: src/data/palette_cycle_175760.cpp

@ gPolarCategoryPalette..0x0817A6B8 (categories 0-2 family data): src/data/anim_family_178f80.cpp

@ gPolarPlayerStateFuncs: src/data/actor_pmf_17a6b8.cpp

@ gPolarPlayerShockPalette..gYetiChargeParams: src/data/actor_tables_17a728.cpp

@ gYetiStateFuncs: src/data/actor_state_fn_17a840.cpp

@ gYetiKeyframes: src/data/anim_frames_17a850.cpp

@ gYetiFrames: src/data/frame_table_17a880.cpp

@ gYetiPalette..0x0817C1C0 (categories 3-6 family data): src/data/anim_family_17aa6c.cpp

@ gJetpackPlayerStateFuncs: src/data/actor_pmf_17c1c0.cpp

@ gJetpackFlashPalettes: src/data/palette_strip_17c200.cpp

@ gJetpackPlaneStateFuncs..gJetpackBomberStateFuncs: src/data/actor_pmf_17c260.cpp

@ gAirshipFireballStateFuncs: src/data/actor_pmf_17c2b8.cpp

@ gAirshipAttacks..gAirshipKeyframes: src/data/weapon_kind_17c2d0.cpp

@ gAirshipStateFuncs: src/data/actor_state_17c3fc.cpp

@ gJetpackBalloonStateFuncs: src/data/actor_pmf_17c414.cpp

@ gJetpackBalloonCrateStateFuncs: src/data/actor_pmf_17c42c.cpp

@ gJetpackRocketBox: src/data/actor_box_17c444.cpp

@ gHovercraftFireballStateFuncs: src/data/actor_pmf_17c450.cpp

@ gHovercraftAttacks..gHovercraftKeyframes: src/data/singleton_kind_17c460.cpp

@ gHovercraftStateFuncs..gHovercraftLauncherStateFuncs: src/data/actor_state_17c4c8.cpp

@ gContinuePromptCursorText..gContinuePromptPalette3: src/data/hud_palettes_17c510.cpp

@ gContinuePromptSmokeBg..gContinuePromptUkaUkaBg: src/data/bg_package_17c594.cpp

@ gCreditsText..gCreditsEmptyText: src/data/credits_17c5d0.cpp

@ gCreditsLogos..gTitleLogoPieceSeeds: src/data/popup_glyphs_17cf40.cpp

@ gTitleArrowPieceOffsets..gLogoActorAnim: src/data/level_gfx_17cff4.cpp

@ gVvLogoPieceSeeds..gVvLogoUrlObj: src/data/slot_seeds_17d6c0.cpp

@ gUniversalLogoBg..0x0817E714: src/data/countdown_17d7a4.cpp

@ gLanguageNames: src/data/digit_glyphs_17e714.cpp

@ gLanguageSelectPalette0..gLanguageSelectPalette2: src/data/palettes_17e72c.cpp

@ gLanguageSelectPalette3..gUnderwaterBg0Tiles: src/data/level_tilesets_17e78c.cpp

@ 0x0824B638..0x08270F08 (33 rooms' level data): src/data/level_rooms_24b638.cpp
@ gSewerBg0Tiles..gSpaceBg0Tiles: src/data/level_tilesets_270f08.cpp
@ 0x082B91D0..0x082BF120 (8 rooms' level data): src/data/level_rooms_2b91d0.cpp
@ gSpriteBank00Tiles..gObjPalettes: src/data/sprite_tiles_2bf120.cpp

@ gSpriteBankTable..gSpriteBank55: src/data/sprite_banks_4a5600.cpp,
@ sprite_banks_4b0ae0.cpp, sprite_banks_4b414c.cpp, sprite_banks_4b9d7c.cpp

.section .rodata.084C0006

.global gGaxSfxData
gGaxSfxData:
	@ Shin'en GAX2 sound-effect data set: 2 bytes of alignment, 88
	@ instruments, 87 8-bit samples, the sample table, and the one handler
	@ type every sound-effect voice uses (its song data points at the
	@ instrument and sample tables). PlaySfx's instrument ids index it.
	@
	@ Built from sound/gax_sfx_manifest.json and sound/sfx_samples/*.wav by
	@ tools/gax_audio.py --sfx; see docs/audio.md.
	.incbin "build/crashbandicootxs/sound/gax_sfx_data.bin"

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
	.incbin "build/crashbandicootxs/sound/gax_audio_data.bin"

.global gGaxDefaultSong
gGaxDefaultSong:
	@ The engine's default handler layout (GAX2_init, GAX2_estimate): the
	@ song struct of a silent one-channel song whose other objects end the
	@ music block. Built with it by tools/gax_audio.py (manifest
	@ `default_song`); see docs/audio.md.
	.incbin "build/crashbandicootxs/sound/gax_default_layout.bin"

@ __clz_tab_divdi3..__clz_tab_udivdi3: lib/libgcc/data/clz_tab_5a4c70.c

@ gSmallFontTiles..gLargeFontTiles: src/data/font_tiles_5a4e70.cpp

@ 0x085A6100..gGaxVibratoTable (GAX2 strings and tables): lib/gax/data/gax_tables_5a6100.c

@ 0x085A9EEC..0x085A9F70 (EEPROM library data): lib/agb_eeprom/data/eeprom_5a9eec.c

@ 0x085A9F70..0x0861BADC (the 24 cutscene pictures): src/data/cutscene_pictures_5a9f70.cpp

@ gMenuSkyBgPalette..gVvLogoUrlTiles: src/data/menu_graphics_61badc.cpp

@ gLevelSelectCursorZoomTiles..gLevelSelectPicture6Palette: src/data/level_select_graphics_6377c0.cpp

@ gSnowBgTiles..gUnderwaterBgTiles: src/data/level_bg_tiles_63d4c0.cpp

@ gRoom00Asset..gRoom33Asset (the 41 level assets): src/data/level_assets_64f830.cpp

@ gEntityVtable..gLogoActorVtable: the C++ classes' vtables, each object's
@ .gnu.linkonce.d._vt.* section placed by ldscript.txt

@ 0x087E55E4..0x087E5FCC: the IWRAM image (asm/intr_main.s, src/iwram/),
@ linked to run at 0x03000000 - the `iwram` section in ldscript.txt.
@ 0x087E5FCC..0x08800000: 0xFF cartridge fill, the `rom_fill` section.

