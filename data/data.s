.section .rodata

@ gCategoryFamily0CellAnim..gCategory0SpawnTable: src/data/cell_anim_03b8b0.cpp

.section .rodata.080B2120

.global gPolarSpriteSheet
gPolarSpriteSheet:
	@ LZ77 compressed data (unidentified) (213064 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/unknown/00_0b2120.bin.lz"

@ gCategory1SpawnTable..gCategory2SpawnTable: src/data/sub_effect_0c0c38.cpp

@ gPolarPlayerRleFrames..gYetiRleFrames: src/data/rle_sprites_0c2758.cpp

@ gCategoryFamily1CellAnim..gCategory3SpawnTable: src/data/cell_anim_0ff1b0.cpp

.section .rodata.0814174C

.global gJetpackSpriteSheet
gJetpackSpriteSheet:
	@ LZ77 compressed data (unidentified) (207124 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/unknown/01_14174c.bin.lz"

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

.section .rodata.085A4E70

.global gSmallFontTiles
gSmallFontTiles:
	@ LZ77 tile graphics (4bpp) (5056 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/00_5a4e70_tiles.4bpp.lz"

.global gLargeFontTiles
gLargeFontTiles:
	@ LZ77 tile graphics (4bpp) (9600 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/00_5a551c_tiles.4bpp.lz"

@ 0x085A6100..gGaxVibratoTable (GAX2 strings and tables): lib/gax/data/gax_tables_5a6100.c

@ 0x085A9EEC..0x085A9F70 (EEPROM library data): lib/agb_eeprom/data/eeprom_5a9eec.c

@ 0x085A9F70..0x0861BADC (the 24 cutscene pictures): src/data/cutscene_pictures_5a9f70.cpp

.section .rodata.0861BADC

.global gMenuSkyBgPalette
gMenuSkyBgPalette:
	@ LZ77 palette (16 colors) (32 bytes decompressed) - the palette for
	@ gMenuSkyBgTiles (sky/clouds background), loaded together as a
	@ {w,h,palette_ptr,tile_ptr,tilemap_ptr} package at gMenuSkyBg
	@ via LoadGraphicsPackage. Originally misclassified as a 1-tile 4bpp graphic (32
	@ bytes coincidentally matches one 4bpp tile).
	.incbin "build/crashbandicootxs/graphics/intro/24_61badc.gbapal.lz", 0, 0x28

.global gPauseMenuBgPalette
gPauseMenuBgPalette:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for gPauseMenuBgTiles (Crash's face in a badge, warp-room
	@ background), loaded as a package at gPauseMenuBg via
	@ LoadGraphicsPackage. Kept as raw binary: some entries have a stray set bit 15
	@ that a standard .pal round-trip through gbagfx can't reproduce
	@ (RGB555 only uses bits 0-14), which broke byte-exact rebuilding when
	@ tried as .pal
	.incbin "build/crashbandicootxs/graphics/intro/25_61bb04.bin.lz", 0, 0x244

.global gLevelSelectPageBgPalette
gLevelSelectPageBgPalette:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for gLevelSelectPageBgTiles (a level-select platform icon: a
	@ blue gem pool, palm trees, small ruins), loaded as a package at
	@ gLevelSelectPageBg via LoadGraphicsPackage. Kept as raw binary: some
	@ entries have a stray set bit 15 that a standard .pal round-trip
	@ through gbagfx can't reproduce (RGB555 only uses bits 0-14), which
	@ broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/intro/26_61bd48.bin.lz"

.global gContinuePromptSmokeBgPalette
gContinuePromptSmokeBgPalette:
	@ LZ77 palette (16 colors) (32 bytes decompressed) - the palette for
	@ gContinuePromptSmokeBgTiles (a red/fiery smoke texture), loaded as a package
	@ at gContinuePromptSmokeBg via LoadGraphicsPackage. Originally misclassified as
	@ a 1-tile 4bpp graphic (32 bytes coincidentally matches one 4bpp tile).
	.incbin "build/crashbandicootxs/graphics/intro/27_61bf30.gbapal.lz", 0, 0x28

.global gContinuePromptGlowBgPalette
gContinuePromptGlowBgPalette:
	@ LZ77 palette (16 colors) (32 bytes decompressed) - the palette for
	@ gContinuePromptGlowBgTiles (a fire/aura glow effect: green transparent
	@ background, orange/red/magenta outline), loaded as a package at
	@ gContinuePromptGlowBg via LoadGraphicsPackage. Originally misclassified as a
	@ 1-tile 4bpp graphic (32 bytes coincidentally matches one 4bpp tile).
	.incbin "build/crashbandicootxs/graphics/intro/28_61bf58.gbapal.lz", 0, 0x28

.global gContinuePromptUkaUkaBgPalette
gContinuePromptUkaUkaBgPalette:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for gContinuePromptUkaUkaBgTiles (Uka Uka's mask), loaded as a
	@ package at gContinuePromptUkaUkaBg via LoadGraphicsPackage. Kept as raw binary:
	@ some entries have a stray set bit 15 that a standard .pal round-trip
	@ through gbagfx can't reproduce (RGB555 only uses bits 0-14), which
	@ broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/intro/29_61bf80.bin.lz", 0, 0x1DC

.global gTitleBandicootObjPalette
gTitleBandicootObjPalette:
	@ LZ77 tile graphics (4bpp) (32 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/30_61c15c_tiles.4bpp.lz"

.global gTitleCrashObjPalette
gTitleCrashObjPalette:
	@ LZ77 tile graphics (4bpp) (32 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/31_61c184_tiles.4bpp.lz", 0, 0x28

.global gTitleArrow1ObjPalette
gTitleArrow1ObjPalette:
	@ LZ77 tile graphics (4bpp) (32 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/32_61c1ac_tiles.4bpp.lz", 0, 0x28

.global gTitleArrow2ObjPalette
gTitleArrow2ObjPalette:
	@ LZ77 tile graphics (4bpp) (32 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/33_61c1d4_tiles.4bpp.lz", 0, 0x28

.global gUniversalLogoBgPalette
gUniversalLogoBgPalette:
	@ LZ77 palette (16 colors) (32 bytes decompressed). This is
	@ gUniversalLogoBg's (the legal/credits text screen) palette - see
	@ gUniversalLogoBgTiles below.
	.incbin "build/crashbandicootxs/graphics/intro/34_61c1fc.gbapal.lz", 0, 0x28

.global gTitleScreenBgPalette
gTitleScreenBgPalette:
	@ LZ77 compressed data (512 bytes decompressed) - a 256-color RGB555
	@ palette, CONFIRMED: this is the palette for gTitleScreenBgTiles (the
	@ "Crash Bandicoot XS" title/logo tile graphics), referenced together
	@ via the graphics package struct at gTitleScreenBg. Kept as raw
	@ binary rather than .pal: some entries have a stray set bit 15 that a
	@ standard .pal round-trip through gbagfx can't reproduce (RGB555 only
	@ uses bits 0-14), which broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/intro/35_61c224.bin.lz"

.global gMenuSkyBgTiles
gMenuSkyBgTiles:
	@ 4bpp tiles of gMenuSkyBg (LZ77): the sky/clouds background (32x20 cells).
	@ Built by grit from graphics/bg_package/61c30c_menu_sky.png,
	@ see docs/graphics.md, "Menu and title pictures".
	.incbin "build/crashbandicootxs/graphics/bg_package/61c30c_menu_sky.img.bin.lz"

.global gPauseMenuBgTiles
gPauseMenuBgTiles:
	@ 8bpp tiles of gPauseMenuBg (LZ77): Crash's face on the warp-room wall (30x20).
	@ Built by grit from graphics/bg_package/61e5f8_pause_menu.png,
	@ see docs/graphics.md, "Menu and title pictures".
	.incbin "build/crashbandicootxs/graphics/bg_package/61e5f8_pause_menu.img.bin.lz"

.global gLevelSelectPageBgTiles
gLevelSelectPageBgTiles:
	@ 8bpp tiles of gLevelSelectPageBg (LZ77): the level-select platform icon (32x32).
	@ Built by grit from graphics/bg_package/62556c_level_select_page.png,
	@ see docs/graphics.md, "Menu and title pictures".
	.incbin "build/crashbandicootxs/graphics/bg_package/62556c_level_select_page.img.bin.lz"

.global gContinuePromptSmokeBgTiles
gContinuePromptSmokeBgTiles:
	@ 4bpp tiles of gContinuePromptSmokeBg (LZ77): the red smoke texture (30x20).
	@ Built by grit from graphics/bg_package/628c50_continue_prompt_smoke.png,
	@ see docs/graphics.md, "Menu and title pictures".
	.incbin "build/crashbandicootxs/graphics/bg_package/628c50_continue_prompt_smoke.img.bin.lz"

.global gContinuePromptGlowBgTiles
gContinuePromptGlowBgTiles:
	@ 4bpp tiles of gContinuePromptGlowBg (LZ77): the fire/aura glow (30x20).
	@ Built by grit from graphics/bg_package/62a958_continue_prompt_glow.png,
	@ see docs/graphics.md, "Menu and title pictures".
	.incbin "build/crashbandicootxs/graphics/bg_package/62a958_continue_prompt_glow.img.bin.lz", 0, 0x9F4

.global gContinuePromptUkaUkaBgTiles
gContinuePromptUkaUkaBgTiles:
	@ 8bpp tiles of gContinuePromptUkaUkaBg (LZ77): Uka Uka's mask (30x20).
	@ Built by grit from graphics/bg_package/62b34c_continue_prompt_uka_uka.png,
	@ see docs/graphics.md, "Menu and title pictures".
	.incbin "build/crashbandicootxs/graphics/bg_package/62b34c_continue_prompt_uka_uka.img.bin.lz"

.global gTitleBandicootObjTiles
gTitleBandicootObjTiles:
	@ 4bpp OBJ tile sheet of gTitleBandicootObj (LZ77): "BANDICOOT" (4x6).
	@ Built by grit from graphics/bg_package/62c2c8_title_bandicoot_obj.png,
	@ see docs/graphics.md, "Menu and title pictures".
	.incbin "build/crashbandicootxs/graphics/bg_package/62c2c8_title_bandicoot_obj.img.bin.lz"

.global gTitleCrashObjTiles
gTitleCrashObjTiles:
	@ 4bpp OBJ tile sheet of gTitleCrashObj (LZ77): the "CRASH" letters (8x40).
	@ Built by grit from graphics/bg_package/62c4b0_title_crash_obj.png,
	@ see docs/graphics.md, "Menu and title pictures".
	.incbin "build/crashbandicootxs/graphics/bg_package/62c4b0_title_crash_obj.img.bin.lz"

.global gTitleArrow1ObjTiles
gTitleArrow1ObjTiles:
	@ 4bpp OBJ tile sheet of gTitleArrow1Obj (LZ77): the first menu arrow (4x16).
	@ Built by grit from graphics/bg_package/62cc3c_title_arrow1_obj.png,
	@ see docs/graphics.md, "Menu and title pictures".
	.incbin "build/crashbandicootxs/graphics/bg_package/62cc3c_title_arrow1_obj.img.bin.lz"

.global gTitleArrow2ObjTiles
gTitleArrow2ObjTiles:
	@ 4bpp OBJ tile sheet of gTitleArrow2Obj (LZ77): the second menu arrow (4x16).
	@ Built by grit from graphics/bg_package/62ce70_title_arrow2_obj.png,
	@ see docs/graphics.md, "Menu and title pictures".
	.incbin "build/crashbandicootxs/graphics/bg_package/62ce70_title_arrow2_obj.img.bin.lz"

.global gUniversalLogoBgTiles
gUniversalLogoBgTiles:
	@ 4bpp tiles of gUniversalLogoBg (LZ77): the legal/credits text screen (30x20).
	@ Built by grit from graphics/bg_package/62d0cc_universal_logo_tiles.png,
	@ see docs/graphics.md, "Menu and title pictures".
	.incbin "build/crashbandicootxs/graphics/bg_package/62d0cc_universal_logo.img.bin.lz"

.global gTitleScreenBgTiles
gTitleScreenBgTiles:
	@ 8bpp tiles of gTitleScreenBg (LZ77): the "Crash Bandicoot XS" title logo (16x16, affine BG2).
	@ Built by grit from graphics/bg_package/62e3b0_title_screen.png,
	@ see docs/graphics.md, "Menu and title pictures".
	.incbin "build/crashbandicootxs/graphics/bg_package/62e3b0_title_screen.img.bin.lz", 0, 0x1774

.global gMenuSkyBgMap
gMenuSkyBgMap:
	@ LZ77 text BG map (u16 entries) of gMenuSkyBg.
	@ Built by grit and tools/bg_map.py from graphics/bg_package/61c30c_menu_sky.png,
	@ see docs/graphics.md, "Menu and title pictures".
	.incbin "build/crashbandicootxs/graphics/bg_package/61c30c_menu_sky.map.bin.lz"

.global gPauseMenuBgMap
gPauseMenuBgMap:
	@ LZ77 text BG map (u16 entries) of gPauseMenuBg.
	@ Built by grit and tools/bg_map.py from graphics/bg_package/61e5f8_pause_menu.png,
	@ see docs/graphics.md, "Menu and title pictures".
	.incbin "build/crashbandicootxs/graphics/bg_package/61e5f8_pause_menu.map.bin.lz", 0, 0x548

.global gLevelSelectPageBgMap
gLevelSelectPageBgMap:
	@ LZ77 text BG map (u16 entries) of gLevelSelectPageBg.
	@ Built by grit and tools/bg_map.py from graphics/bg_package/62556c_level_select_page.png,
	@ see docs/graphics.md, "Menu and title pictures".
	.incbin "build/crashbandicootxs/graphics/bg_package/62556c_level_select_page.map.bin.lz"

.global gContinuePromptSmokeBgMap
gContinuePromptSmokeBgMap:
	@ LZ77 text BG map (u16 entries) of gContinuePromptSmokeBg.
	@ Built by grit and tools/bg_map.py from graphics/bg_package/628c50_continue_prompt_smoke.png,
	@ see docs/graphics.md, "Menu and title pictures".
	.incbin "build/crashbandicootxs/graphics/bg_package/628c50_continue_prompt_smoke.map.bin.lz"

.global gContinuePromptGlowBgMap
gContinuePromptGlowBgMap:
	@ LZ77 text BG map (u16 entries) of gContinuePromptGlowBg.
	@ Built by grit and tools/bg_map.py from graphics/bg_package/62a958_continue_prompt_glow.png,
	@ see docs/graphics.md, "Menu and title pictures".
	.incbin "build/crashbandicootxs/graphics/bg_package/62a958_continue_prompt_glow.map.bin.lz"

.global gContinuePromptUkaUkaBgMap
gContinuePromptUkaUkaBgMap:
	@ LZ77 text BG map (u16 entries) of gContinuePromptUkaUkaBg.
	@ Built by grit and tools/bg_map.py from graphics/bg_package/62b34c_continue_prompt_uka_uka.png,
	@ see docs/graphics.md, "Menu and title pictures".
	.incbin "build/crashbandicootxs/graphics/bg_package/62b34c_continue_prompt_uka_uka.map.bin.lz"

.global gTitleBandicootObjMap
gTitleBandicootObjMap:
	@ LZ77 OBJ tile map of gTitleBandicootObj: a u16 per OBJ tile, whose low
	@ byte picks the sheet tile (TitleScreen::LoadObjTiles). Built by grit and
	@ tools/bg_map.py from graphics/bg_package/62c2c8_title_bandicoot_obj.png.
	.incbin "build/crashbandicootxs/graphics/bg_package/62c2c8_title_bandicoot_obj.map.bin.lz"

.global gTitleCrashObjMap
gTitleCrashObjMap:
	@ LZ77 OBJ tile map of gTitleCrashObj: a u16 per OBJ tile, whose low
	@ byte picks the sheet tile (TitleScreen::LoadObjTiles). Built by grit and
	@ tools/bg_map.py from graphics/bg_package/62c4b0_title_crash_obj.png.
	.incbin "build/crashbandicootxs/graphics/bg_package/62c4b0_title_crash_obj.map.bin.lz", 0, 0x140

.global gTitleArrow1ObjMap
gTitleArrow1ObjMap:
	@ LZ77 OBJ tile map of gTitleArrow1Obj: a u16 per OBJ tile, whose low
	@ byte picks the sheet tile (TitleScreen::LoadObjTiles). Built by grit and
	@ tools/bg_map.py from graphics/bg_package/62cc3c_title_arrow1_obj.png.
	.incbin "build/crashbandicootxs/graphics/bg_package/62cc3c_title_arrow1_obj.map.bin.lz"

.global gTitleArrow2ObjMap
gTitleArrow2ObjMap:
	@ LZ77 OBJ tile map of gTitleArrow2Obj: a u16 per OBJ tile, whose low
	@ byte picks the sheet tile (TitleScreen::LoadObjTiles). Built by grit and
	@ tools/bg_map.py from graphics/bg_package/62ce70_title_arrow2_obj.png.
	.incbin "build/crashbandicootxs/graphics/bg_package/62ce70_title_arrow2_obj.map.bin.lz"

.global gUniversalLogoBgMap
gUniversalLogoBgMap:
	@ LZ77 text BG map (u16 entries) of gUniversalLogoBg.
	@ Built by grit and tools/bg_map.py from graphics/bg_package/62d0cc_universal_logo.png,
	@ see docs/graphics.md, "Menu and title pictures".
	.incbin "build/crashbandicootxs/graphics/bg_package/62d0cc_universal_logo.map.bin.lz"

.global gTitleScreenBgMap
gTitleScreenBgMap:
	@ LZ77 affine map (u16 entries, LoadBg keeps the low byte) of gTitleScreenBg.
	@ Built by grit and tools/bg_map.py from graphics/bg_package/62e3b0_title_screen.png,
	@ see docs/graphics.md, "Menu and title pictures".
	.incbin "build/crashbandicootxs/graphics/bg_package/62e3b0_title_screen.map.bin.lz"

.global gCreditsRedEyeStudiosLogoPalette
gCreditsRedEyeStudiosLogoPalette:
	@ LZ77 tile graphics (4bpp) (32 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/60_6319a0_tiles.4bpp.lz", 0, 0x28

.global gCreditsShinenLogoPalette
gCreditsShinenLogoPalette:
	@ LZ77 tile graphics (4bpp) (32 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/61_6319c8_tiles.4bpp.lz", 0, 0x28

.global gCreditsCosmigoLogoPalette
gCreditsCosmigoLogoPalette:
	@ LZ77 tile graphics (4bpp) (32 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/62_6319f0_tiles.4bpp.lz", 0, 0x28

.global gCreditsUniversalLogoPalette
gCreditsUniversalLogoPalette:
	@ LZ77 tile graphics (4bpp) (32 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/63_631a18_tiles.4bpp.lz", 0, 0x28

.global gCreditsVvLogoPalette
gCreditsVvLogoPalette:
	@ LZ77 tile graphics (4bpp) (32 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/64_631a40_tiles.4bpp.lz", 0, 0x28

.global gVvLogoEmblemPalette
gVvLogoEmblemPalette:
	@ LZ77 tile graphics (4bpp) (32 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/65_631a68_tiles.4bpp.lz", 0, 0x28

.global gVvLogoLettersPalette
gVvLogoLettersPalette:
	@ LZ77 tile graphics (4bpp) (32 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/66_631a90_tiles.4bpp.lz", 0, 0x28

.global gVvLogoUrlPalette
gVvLogoUrlPalette:
	@ LZ77 tile graphics (4bpp) (32 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/67_631ab8_tiles.4bpp.lz", 0, 0x14

.global gCreditsRedEyeStudiosLogoTiles
gCreditsRedEyeStudiosLogoTiles:
	@ LZ77 tile graphics (4bpp) (4608 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/68_631acc_tiles.4bpp.lz"

.global gCreditsShinenLogoTiles
gCreditsShinenLogoTiles:
	@ LZ77 tile graphics (4bpp) (2048 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/69_6324b4_tiles.4bpp.lz"

.global gCreditsCosmigoLogoTiles
gCreditsCosmigoLogoTiles:
	@ LZ77 tile graphics (4bpp) (2048 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/70_632820_tiles.4bpp.lz"

.global gCreditsUniversalLogoTiles
gCreditsUniversalLogoTiles:
	@ LZ77 tile graphics (4bpp) (6144 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/71_632bc4_tiles.4bpp.lz"

.global gCreditsVvLogoTiles
gCreditsVvLogoTiles:
	@ LZ77 tile graphics (4bpp) (7680 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/72_6334c4_tiles.4bpp.lz", 0, 0xDAC

.global gVvLogoEmblemTiles
gVvLogoEmblemTiles:
	@ LZ77 tile graphics (4bpp) (25600 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/73_634270_tiles.4bpp.lz"

.global gVvLogoLettersTiles
gVvLogoLettersTiles:
	@ LZ77 tile graphics (4bpp) (4608 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/74_636ef4_tiles.4bpp.lz"

.global gVvLogoUrlTiles
gVvLogoUrlTiles:
	@ LZ77 tile graphics (4bpp) (1024 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/75_637604_tiles.4bpp.lz"

.global gLevelSelectCursorZoomTiles
gLevelSelectCursorZoomTiles:
	@ LZ77 tile graphics (4bpp) (2048 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/00_6377c0_tiles.4bpp.lz"

.global gLevelSelectPicture0Tiles
gLevelSelectPicture0Tiles:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gLevelSelectPicture0Palette
	.incbin "build/crashbandicootxs/graphics/tileset1/01_637a70_8bpp_tiles.8bpp.lz"

.global gLevelSelectPicture1Tiles
gLevelSelectPicture1Tiles:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gLevelSelectPicture1Palette
	.incbin "build/crashbandicootxs/graphics/tileset1/02_6382c8_8bpp_tiles.8bpp.lz", 0, 0xAA0

.global gLevelSelectPicture4Tiles
gLevelSelectPicture4Tiles:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gLevelSelectPicture4Palette
	.incbin "build/crashbandicootxs/graphics/tileset1/03_638d68_8bpp_tiles.8bpp.lz"

.global gLevelSelectPicture2Tiles
gLevelSelectPicture2Tiles:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gLevelSelectPicture2Palette
	.incbin "build/crashbandicootxs/graphics/tileset1/04_63945c_8bpp_tiles.8bpp.lz", 0, 0x8BC

.global gLevelSelectPicture5Tiles
gLevelSelectPicture5Tiles:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gLevelSelectPicture5Palette
	.incbin "build/crashbandicootxs/graphics/tileset1/05_639d18_8bpp_tiles.8bpp.lz"

.global gLevelSelectPicture3Tiles
gLevelSelectPicture3Tiles:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gLevelSelectPicture3Palette
	.incbin "build/crashbandicootxs/graphics/tileset1/06_63a60c_8bpp_tiles.8bpp.lz"

.global gLevelSelectPicture9Tiles
gLevelSelectPicture9Tiles:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gLevelSelectPicture9Palette
	.incbin "build/crashbandicootxs/graphics/tileset1/07_63ad90_8bpp_tiles.8bpp.lz"

.global gLevelSelectPicture7Tiles
gLevelSelectPicture7Tiles:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gLevelSelectPicture7Palette
	.incbin "build/crashbandicootxs/graphics/tileset1/08_63b668_8bpp_tiles.8bpp.lz"

.global gLevelSelectPicture8Tiles
gLevelSelectPicture8Tiles:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gLevelSelectPicture8Palette
	.incbin "build/crashbandicootxs/graphics/tileset1/09_63bdd4_8bpp_tiles.8bpp.lz", 0, 0x810

.global gLevelSelectPicture6Tiles
gLevelSelectPicture6Tiles:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gLevelSelectPicture6Palette
	.incbin "build/crashbandicootxs/graphics/tileset1/10_63c5e4_8bpp_tiles.8bpp.lz"

.global gLevelSelectPicture0Palette
gLevelSelectPicture0Palette:
	@ LZ77 palette (256 colors) (512 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/11_63cf98.gbapal.lz", 0, 0x84

.global gLevelSelectPicture1Palette
gLevelSelectPicture1Palette:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for the circular level-select icon at gLevelSelectPicture1Tiles.
	@ Kept as raw binary: some entries have a stray set bit 15 that a
	@ standard .pal round-trip through gbagfx can't reproduce (RGB555 only
	@ uses bits 0-14), which broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/tileset1/12_63d01c.bin.lz", 0, 0x84

.global gLevelSelectPicture4Palette
gLevelSelectPicture4Palette:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for the circular level-select icon at gLevelSelectPicture4Tiles.
	@ Kept as raw binary: some entries have a stray set bit 15 that a
	@ standard .pal round-trip through gbagfx can't reproduce (RGB555 only
	@ uses bits 0-14), which broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/tileset1/13_63d0a0.bin.lz", 0, 0x84

.global gLevelSelectPicture2Palette
gLevelSelectPicture2Palette:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for the circular level-select icon at gLevelSelectPicture2Tiles.
	@ Kept as raw binary: some entries have a stray set bit 15 that a
	@ standard .pal round-trip through gbagfx can't reproduce (RGB555 only
	@ uses bits 0-14), which broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/tileset1/14_63d124.bin.lz", 0, 0x84

.global gLevelSelectPicture5Palette
gLevelSelectPicture5Palette:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for the circular level-select icon at gLevelSelectPicture5Tiles.
	@ Kept as raw binary: some entries have a stray set bit 15 that a
	@ standard .pal round-trip through gbagfx can't reproduce (RGB555 only
	@ uses bits 0-14), which broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/tileset1/15_63d1a8.bin.lz", 0, 0x84

.global gLevelSelectPicture3Palette
gLevelSelectPicture3Palette:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for the circular level-select icon at gLevelSelectPicture3Tiles.
	@ Kept as raw binary: some entries have a stray set bit 15 that a
	@ standard .pal round-trip through gbagfx can't reproduce (RGB555 only
	@ uses bits 0-14), which broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/tileset1/16_63d22c.bin.lz", 0, 0x84

.global gLevelSelectPicture9Palette
gLevelSelectPicture9Palette:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for the circular level-select icon at gLevelSelectPicture9Tiles.
	@ Kept as raw binary: some entries have a stray set bit 15 that a
	@ standard .pal round-trip through gbagfx can't reproduce (RGB555 only
	@ uses bits 0-14), which broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/tileset1/17_63d2b0.bin.lz"

.global gLevelSelectPicture7Palette
gLevelSelectPicture7Palette:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for the circular level-select icon at gLevelSelectPicture7Tiles.
	@ Kept as raw binary: some entries have a stray set bit 15 that a
	@ standard .pal round-trip through gbagfx can't reproduce (RGB555 only
	@ uses bits 0-14), which broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/tileset1/18_63d334.bin.lz", 0, 0x84

.global gLevelSelectPicture8Palette
gLevelSelectPicture8Palette:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for the circular level-select icon at gLevelSelectPicture8Tiles.
	@ Kept as raw binary: some entries have a stray set bit 15 that a
	@ standard .pal round-trip through gbagfx can't reproduce (RGB555 only
	@ uses bits 0-14), which broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/tileset1/19_63d3b8.bin.lz", 0, 0x84

.global gLevelSelectPicture6Palette
gLevelSelectPicture6Palette:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for the circular level-select icon at gLevelSelectPicture6Tiles.
	@ Kept as raw binary: some entries have a stray set bit 15 that a
	@ standard .pal round-trip through gbagfx can't reproduce (RGB555 only
	@ uses bits 0-14), which broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/tileset1/20_63d43c.bin.lz"

.global gSnowBgTiles
gSnowBgTiles:
	@ LZ77 tile graphics (4bpp) (23520 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/21_63d4c0_tiles.4bpp.lz"

.global gJungleBgTiles
gJungleBgTiles:
	@ LZ77 tile graphics (4bpp) (23392 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/22_640ef8_tiles.4bpp.lz"

.global gTempleBgTiles
gTempleBgTiles:
	@ LZ77 compressed data (24352 bytes decompressed) - not clearly identifiable as pixel graphics
	.incbin "build/crashbandicootxs/graphics/tileset1/23_644a88.bin.lz"

.global gSewerBgTiles
gSewerBgTiles:
	@ LZ77 compressed data (15968 bytes decompressed) - not clearly identifiable as pixel graphics
	.incbin "build/crashbandicootxs/graphics/tileset1/24_649418.bin.lz", 0, 0x2744

.global gSpaceBgTiles
gSpaceBgTiles:
	@ LZ77 compressed data (12576 bytes decompressed) - not clearly identifiable as pixel graphics
	.incbin "build/crashbandicootxs/graphics/tileset1/25_64bb5c.bin.lz"

.global gUnderwaterBgTiles
gUnderwaterBgTiles:
	@ LZ77 compressed data (14240 bytes decompressed) - not clearly identifiable as pixel graphics
	.incbin "build/crashbandicootxs/graphics/tileset1/26_64d7e8.bin.lz"

.global gRoom00Asset
gRoom00Asset:
	@ LZ77-packed level asset of room00_264730 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room00_264730/asset.bin.lz"

.global gRoom01Asset
gRoom01Asset:
	@ LZ77-packed level asset of room01_263f4c (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room01_263f4c/asset.bin.lz"

.global gRoom02Asset
gRoom02Asset:
	@ LZ77-packed level asset of room02_2636e0 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room02_2636e0/asset.bin.lz", 0, 0x1BE8

.global gRoom04Asset
gRoom04Asset:
	@ LZ77-packed level asset of room04_262e64 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room04_262e64/asset.bin.lz"

.global gRoom06Asset
gRoom06Asset:
	@ LZ77-packed level asset of room06_267428 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room06_267428/asset.bin.lz"

.global gRoom07Asset
gRoom07Asset:
	@ LZ77-packed level asset of room07_264e64 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room07_264e64/asset.bin.lz"

.global gRoom09Asset
gRoom09Asset:
	@ LZ77-packed level asset of room09_262654 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room09_262654/asset.bin.lz"

.global gRoom38Asset
gRoom38Asset:
	@ LZ77-packed level asset of room38_2bac1c (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room38_2bac1c/asset.bin.lz"

.global gRoom13Asset
gRoom13Asset:
	@ LZ77-packed level asset of room13_2bbde0 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room13_2bbde0/asset.bin.lz", 0, 0x4300

.global gRoom14Asset
gRoom14Asset:
	@ LZ77-packed level asset of room14_2bcf40 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room14_2bcf40/asset.bin.lz", 0, 0x4AC0

.global gRoom16Asset
gRoom16Asset:
	@ LZ77-packed level asset of room16_2beadc (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room16_2beadc/asset.bin.lz"

.global gRoom18Asset
gRoom18Asset:
	@ LZ77-packed level asset of room18_261c40 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room18_261c40/asset.bin.lz"

.global gRoom20Asset
gRoom20Asset:
	@ LZ77-packed level asset of room20_25f11c (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room20_25f11c/asset.bin.lz", 0, 0x3508

.global gRoom21Asset
gRoom21Asset:
	@ LZ77-packed level asset of room21_25d81c (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room21_25d81c/asset.bin.lz"

.global gRoom23Asset
gRoom23Asset:
	@ LZ77-packed level asset of room23_25cf20 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room23_25cf20/asset.bin.lz"

.global gRoom24Asset
gRoom24Asset:
	@ LZ77-packed level asset of room24_25c640 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room24_25c640/asset.bin.lz", 0, 0x2F54

.global gRoom28Asset
gRoom28Asset:
	@ LZ77-packed level asset of room28_254ed0 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room28_254ed0/asset.bin.lz"

.global gRoom29Asset
gRoom29Asset:
	@ LZ77-packed level asset of room29_2544fc (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room29_2544fc/asset.bin.lz"

.global gRoom30Asset
gRoom30Asset:
	@ LZ77-packed level asset of room30_26ac10 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room30_26ac10/asset.bin.lz"

.global gRoom31Asset
gRoom31Asset:
	@ LZ77-packed level asset of room31_2539b0 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room31_2539b0/asset.bin.lz", 0, 0x9C74

.global gRoom34Asset
gRoom34Asset:
	@ LZ77-packed level asset of room34_24c400 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room34_24c400/asset.bin.lz"

.global gRoom35Asset
gRoom35Asset:
	@ LZ77-packed level asset of room35_26c1bc (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room35_26c1bc/asset.bin.lz"

.global gRoom36Asset
gRoom36Asset:
	@ LZ77-packed level asset of room36_268cf0 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room36_268cf0/asset.bin.lz", 0, 0x6468

.global gRoom22Asset
gRoom22Asset:
	@ LZ77-packed level asset of room22_26e760 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room22_26e760/asset.bin.lz"

.global gRoom25Asset
gRoom25Asset:
	@ LZ77-packed level asset of room25_26d388 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room25_26d388/asset.bin.lz", 0, 0x65C8

.global gRoom17Asset
gRoom17Asset:
	@ raw level asset of room17_25e7dc (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room17_25e7dc/asset.bin"

.global gRoom37Asset
gRoom37Asset:
	@ LZ77-packed level asset of room37_2b9ed0 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room37_2b9ed0/asset.bin.lz"

.global gRoom40Asset
gRoom40Asset:
	@ LZ77-packed level asset of room40_2bb094 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room40_2bb094/asset.bin.lz", 0, 0x988

.global gRoom03Asset
gRoom03Asset:
	@ LZ77-packed level asset of room03_270bcc (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room03_270bcc/asset.bin.lz"

.global gRoom05Asset
gRoom05Asset:
	@ LZ77-packed level asset of room05_270154 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room05_270154/asset.bin.lz"

.global gRoom11Asset
gRoom11Asset:
	@ LZ77-packed level asset of room11_26f5c0 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room11_26f5c0/asset.bin.lz", 0, 0x120C

.global gRoom08Asset
gRoom08Asset:
	@ LZ77-packed level asset of room08_265804 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room08_265804/asset.bin.lz", 0, 0x2908

.global gRoom10Asset
gRoom10Asset:
	@ LZ77-packed level asset of room10_266194 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room10_266194/asset.bin.lz"

.global gRoom12Asset
gRoom12Asset:
	@ LZ77-packed level asset of room12_266b40 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room12_266b40/asset.bin.lz", 0, 0x2570

.global gRoom39Asset
gRoom39Asset:
	@ LZ77-packed level asset of room39_2ba810 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room39_2ba810/asset.bin.lz"

.global gRoom15Asset
gRoom15Asset:
	@ raw level asset of room15_2bdf98 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room15_2bdf98/asset.bin"

.global gRoom19Asset
gRoom19Asset:
	@ raw level asset of room19_260768 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room19_260768/asset.bin"

.global gRoom26Asset
gRoom26Asset:
	@ raw level asset of room26_25bcdc (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room26_25bcdc/asset.bin"

.global gRoom27Asset
gRoom27Asset:
	@ raw level asset of room27_25a390 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room27_25a390/asset.bin"

.global gRoom32Asset
gRoom32Asset:
	@ raw level asset of room32_24e104 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room32_24e104/asset.bin"

.global gRoom33Asset
gRoom33Asset:
	@ raw level asset of room33_25233c (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room33_25233c/asset.bin"

@ gEntityVtable..gLogoActorVtable: the C++ classes' vtables, each object's
@ .gnu.linkonce.d._vt.* section placed by ldscript.txt

@ 0x087E55E4..0x087E5FCC: the IWRAM image (asm/intr_main.s, src/iwram/),
@ linked to run at 0x03000000 - the `iwram` section in ldscript.txt.
@ 0x087E5FCC..0x08800000: 0xFF cartridge fill, the `rom_fill` section.

