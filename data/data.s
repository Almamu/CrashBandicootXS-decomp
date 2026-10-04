.section .rodata

@ gCategoryFamily0CellAnim..gCategory0SpawnTable: src/data/cell_anim_03b8b0.c

.section .rodata.080B2120

.global gStaticData_080B2120
gStaticData_080B2120:
	@ LZ77 compressed data (unidentified) (213064 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/unknown/00_0b2120.bin.lz"

@ gCategory1SpawnTable..gCategory2SpawnTable: src/data/sub_effect_0c0c38.c

@ gStaticData_080C2758..gStaticData_080DA1D8: src/data/rle_sprites_0c2758.c

@ gCategoryFamily1CellAnim..gCategory3SpawnTable: src/data/cell_anim_0ff1b0.c

.section .rodata.0814174C

.global gStaticData_0814174C
gStaticData_0814174C:
	@ LZ77 compressed data (unidentified) (207124 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/unknown/01_14174c.bin.lz"

@ gCategory4BgPicture..gCategory6SpawnTable: src/data/bg_picture_151ac4.c

@ gStaticData_0815A050: src/data/rle_sprites_15a050.c

@ gStaticData_08167AD4..gSineTable: src/data/boss_pictures_167ad4.c

@ gSongTable: src/data/song_table_16aa20.c

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

@ gStaticData_0816AF10..gStaticData_0816B124: src/data/link_crc_16af10.c

@ gStaticData_0816B138..gStaticData_0816B27C: src/data/menu_tables_16b138.c

@ gStaticData_0816B284: src/data/bg_package_16b284.c

@ gPauseMenuRows..gStaticData_0816B2C0: src/data/pause_rows_16b298.c

@ gObjPieceWidths..gStaticData_0816B300: src/data/obj_sizes_16b2e0.c

@ gStaticData_0816B304..gStaticData_0816B8C0: src/data/motion_records_16b304.c

@ gStaticData_0816B92C..gStaticData_0816B934: src/data/entry_set_16b92c.c

@ gStaticData_0816B93C..gStaticData_0816B944: src/data/entry_set_16b93c.c

@ gEnemyDefaultAnimMap..gSaucerLabAssistantAnimMap: src/data/popup_tables_16b98c.c

@ gStaticData_0816BB6C..gStaticData_0816BF14: src/data/object_tables_16bb6c.c

@ gStaticData_0816BF20..gStaticData_0816C070: src/data/action_table_16bf20.c

@ gStaticData_0816C090..gStaticData_0816C0B0: src/data/speed_table_16c090.c

@ gStaticData_0816C250..gStaticData_0816C2D0: src/data/player_pmf_16c250.c

@ gStaticData_0816C2D8..gStaticData_0816C3F4: src/data/actor_tables_16c2d8.c

@ gStaticData_0816C418..gStaticData_0816C458: src/data/entry_set_16c418.c

@ gStaticData_0816C460: src/data/velocity_16c460.c

@ gStaticData_0816C484: src/data/bg_package_16c484.c

@ gStaticData_0816C498..gStaticData_0816C56C: src/data/map_tables_16c498.c

@ gStaticData_0816C58C: src/data/bg_package_16c58c.c

@ gStaticData_0816C5F0..gObjSizeHeights: src/data/map_tables_16c5f0.c

@ gEntitySpawnFuncs: src/data/dispatch_table_16c6a4.c

@ gThemePaletteCycle2..gThemeMusicCues: src/data/level_table_16c814.c

@ gCutsceneTextEnglish..gCutscenes: src/data/cutscenes_16d1c8.c

@ gTerrainTypes..0x08172CD4 (terrain types): src/data/terrain_1725a8.c

@ 0x08172CD4..0x08174BE0 (the UI text of six languages): src/data/ui_text_172cd4.c

@ gHudPartAnims..gLargeFontGlyphs: src/data/hud_fonts_174be0.c

@ gActorCategories..gActorCategoryVtables: src/data/actor_category_175558.c

@ gStaticData_08175760..gStaticData_08178F70: src/data/palette_cycle_175760.c

@ gStaticData_08178F80..0x0817A6B8 (categories 0-2 family data): src/data/anim_family_178f80.c

@ gStaticData_0817A6B8: src/data/actor_pmf_17a6b8.c

@ gStaticData_0817A728..gStaticData_0817A7F8: src/data/actor_tables_17a728.c

@ gStaticData_0817A840: src/data/actor_state_fn_17a840.c

@ gStaticData_0817A850: src/data/anim_frames_17a850.c

@ gStaticData_0817A880: src/data/frame_table_17a880.c

@ gStaticData_0817AA6C..0x0817C1C0 (categories 3-6 family data): src/data/anim_family_17aa6c.c

@ gStaticData_0817C1C0: src/data/actor_pmf_17c1c0.c

@ gStaticData_0817C200: src/data/palette_strip_17c200.c

@ gStaticData_0817C260..gStaticData_0817C2B8: src/data/actor_pmf_17c260.c

@ gStaticData_0817C2D0..gStaticData_0817C3E4: src/data/weapon_kind_17c2d0.c

@ gStaticData_0817C3FC..gStaticData_0817C42C: src/data/actor_state_17c3fc.c

@ gStaticData_0817C444: src/data/actor_box_17c444.c

@ gStaticData_0817C450: src/data/actor_pmf_17c450.c

@ gStaticData_0817C460..gStaticData_0817C4BC: src/data/singleton_kind_17c460.c

@ gStaticData_0817C4C8..gStaticData_0817C4F8: src/data/actor_state_17c4c8.c

@ gStaticData_0817C510..gStaticData_0817C572: src/data/hud_palettes_17c510.c

@ gStaticData_0817C594..gStaticData_0817C5BC: src/data/bg_package_17c594.c

@ gCreditsText..gStaticData_0817CF3C: src/data/credits_17c5d0.c

@ gCreditsLogos..gTitleLogoPieceSeeds: src/data/popup_glyphs_17cf40.c

@ gTitleArrowPieceOffsets..gLogoActorAnim: src/data/level_gfx_17cff4.c

@ gVvLogoPieceSeeds..gStaticData_0817D790: src/data/slot_seeds_17d6c0.c

@ gUniversalLogoBg..0x0817E714: src/data/countdown_17d7a4.c

@ gLanguageNames: src/data/digit_glyphs_17e714.c

@ gStaticData_0817E72C..gStaticData_0817E76C: src/data/palettes_17e72c.c

@ gStaticData_0817E78C..gStaticData_08200DF4: src/data/level_tilesets_17e78c.c

@ 0x0824B638..0x08270F08 (33 rooms' level data): src/data/level_rooms_24b638.c
@ gStaticData_08270F08..gStaticData_08299DCC: src/data/level_tilesets_270f08.c
@ 0x082B91D0..0x082BF120 (8 rooms' level data): src/data/level_rooms_2b91d0.c
@ gSpriteBank00Tiles..gFixedObjTiles: src/data/sprite_tiles_2bf120.c

@ gSpriteBankTable..gSpriteBank55: src/data/sprite_banks_4a5600.c,
@ sprite_banks_4b0ae0.c, sprite_banks_4b414c.c, sprite_banks_4b9d7c.c

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

@ gStaticData_085A4C70..gStaticData_085A4D70: src/data/clz_tab_5a4c70.c

.section .rodata.085A4E70

.global gSmallFontTiles
gSmallFontTiles:
	@ LZ77 tile graphics (4bpp) (5056 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/00_5a4e70_tiles.4bpp.lz"

.global gLargeFontTiles
gLargeFontTiles:
	@ LZ77 tile graphics (4bpp) (9600 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/00_5a551c_tiles.4bpp.lz"

@ 0x085A6100..gGaxVibratoTable (GAX2 strings and tables): src/data/gax_tables_5a6100.c

@ 0x085A9EEC..0x085A9F70 (EEPROM library data): src/data/eeprom_5a9eec.c

@ 0x085A9F70..0x0861BADC (the 24 cutscene pictures): src/data/cutscene_pictures_5a9f70.c

.section .rodata.0861BADC

.global gStaticData_0861BADC
gStaticData_0861BADC:
	@ LZ77 palette (16 colors) (32 bytes decompressed) - the palette for
	@ gStaticData_0861C30C (sky/clouds background), loaded together as a
	@ {w,h,palette_ptr,tile_ptr,tilemap_ptr} package at gStaticData_0816C484
	@ via LoadGraphicsPackage. Originally misclassified as a 1-tile 4bpp graphic (32
	@ bytes coincidentally matches one 4bpp tile).
	.incbin "build/crashbandicootxs/graphics/intro/24_61badc.gbapal.lz", 0, 0x28

.global gStaticData_0861BB04
gStaticData_0861BB04:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for gStaticData_0861E5F8 (Crash's face in a badge, warp-room
	@ background), loaded as a package at gStaticData_0816B284 via
	@ LoadGraphicsPackage. Kept as raw binary: some entries have a stray set bit 15
	@ that a standard .pal round-trip through gbagfx can't reproduce
	@ (RGB555 only uses bits 0-14), which broke byte-exact rebuilding when
	@ tried as .pal
	.incbin "build/crashbandicootxs/graphics/intro/25_61bb04.bin.lz", 0, 0x244

.global gStaticData_0861BD48
gStaticData_0861BD48:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for gStaticData_0862556C (a level-select platform icon: a
	@ blue gem pool, palm trees, small ruins), loaded as a package at
	@ gStaticData_0816C58C via LoadGraphicsPackage. Kept as raw binary: some
	@ entries have a stray set bit 15 that a standard .pal round-trip
	@ through gbagfx can't reproduce (RGB555 only uses bits 0-14), which
	@ broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/intro/26_61bd48.bin.lz"

.global gStaticData_0861BF30
gStaticData_0861BF30:
	@ LZ77 palette (16 colors) (32 bytes decompressed) - the palette for
	@ gStaticData_08628C50 (a red/fiery smoke texture), loaded as a package
	@ at gStaticData_0817C594 via LoadGraphicsPackage. Originally misclassified as
	@ a 1-tile 4bpp graphic (32 bytes coincidentally matches one 4bpp tile).
	.incbin "build/crashbandicootxs/graphics/intro/27_61bf30.gbapal.lz", 0, 0x28

.global gStaticData_0861BF58
gStaticData_0861BF58:
	@ LZ77 palette (16 colors) (32 bytes decompressed) - the palette for
	@ gStaticData_0862A958 (a fire/aura glow effect: green transparent
	@ background, orange/red/magenta outline), loaded as a package at
	@ gStaticData_0817C5A8 via LoadGraphicsPackage. Originally misclassified as a
	@ 1-tile 4bpp graphic (32 bytes coincidentally matches one 4bpp tile).
	.incbin "build/crashbandicootxs/graphics/intro/28_61bf58.gbapal.lz", 0, 0x28

.global gStaticData_0861BF80
gStaticData_0861BF80:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for gStaticData_0862B34C (Uka Uka's mask), loaded as a
	@ package at gStaticData_0817C5BC via LoadGraphicsPackage. Kept as raw binary:
	@ some entries have a stray set bit 15 that a standard .pal round-trip
	@ through gbagfx can't reproduce (RGB555 only uses bits 0-14), which
	@ broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/intro/29_61bf80.bin.lz", 0, 0x1DC

.global gStaticData_0861C15C
gStaticData_0861C15C:
	@ LZ77 tile graphics (4bpp) (32 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/30_61c15c_tiles.4bpp.lz"

.global gStaticData_0861C184
gStaticData_0861C184:
	@ LZ77 tile graphics (4bpp) (32 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/31_61c184_tiles.4bpp.lz", 0, 0x28

.global gStaticData_0861C1AC
gStaticData_0861C1AC:
	@ LZ77 tile graphics (4bpp) (32 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/32_61c1ac_tiles.4bpp.lz", 0, 0x28

.global gStaticData_0861C1D4
gStaticData_0861C1D4:
	@ LZ77 tile graphics (4bpp) (32 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/33_61c1d4_tiles.4bpp.lz", 0, 0x28

.global gStaticData_0861C1FC
gStaticData_0861C1FC:
	@ LZ77 palette (16 colors) (32 bytes decompressed). This is
	@ gUniversalLogoBg's (the legal/credits text screen) palette - see
	@ gStaticData_0862D0CC below.
	.incbin "build/crashbandicootxs/graphics/intro/34_61c1fc.gbapal.lz", 0, 0x28

.global gStaticData_0861C224
gStaticData_0861C224:
	@ LZ77 compressed data (512 bytes decompressed) - a 256-color RGB555
	@ palette, CONFIRMED: this is the palette for gStaticData_0862E3B0 (the
	@ "Crash Bandicoot XS" title/logo tile graphics), referenced together
	@ via the graphics package struct at gTitleScreenBg. Kept as raw
	@ binary rather than .pal: some entries have a stray set bit 15 that a
	@ standard .pal round-trip through gbagfx can't reproduce (RGB555 only
	@ uses bits 0-14), which broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/intro/35_61c224.bin.lz"

.global gStaticData_0861C30C
gStaticData_0861C30C:
	@ LZ77 tile graphics (4bpp) (16288 bytes decompressed, 509 tiles - a
	@ prime tile count, so stored as a 1-tile-tall strip). A sky/clouds
	@ background: composited with its real palette (gStaticData_0861BADC)
	@ and tilemap (gStaticData_0862FB24, 32x20 tiles) via the package at
	@ gStaticData_0816C484. Originally left as raw binary ("not clearly
	@ identifiable") since a bare tileset doesn't look like anything on its
	@ own without the tilemap.
	.incbin "build/crashbandicootxs/graphics/intro/36_61c30c_tiles.4bpp.lz"

.global gStaticData_0861E5F8
gStaticData_0861E5F8:
	@ LZ77 tile graphics (8bpp) (38400 bytes decompressed, 600 tiles - an
	@ exact 30x20 screen's worth, no reuse). Crash's face in a blue badge
	@ over a metallic warp-room background: composited with its real
	@ palette (gStaticData_0861BB04) and tilemap (gStaticData_0862FFF4)
	@ via the package at gStaticData_0816B284. Originally misclassified as
	@ a Mode 4 (linear/non-tiled) bitmap - it happens to be exactly
	@ 240x160 like a real Mode 4 bitmap (30x20 tiles x 8px), but it's
	@ genuine tiled+tilemapped BG graphics.
	.incbin "build/crashbandicootxs/graphics/intro/37_61e5f8_8bpp_tiles.8bpp.lz"

.global gStaticData_0862556C
gStaticData_0862556C:
	@ LZ77 tile graphics (8bpp) (20288 bytes decompressed, 317 tiles). A
	@ level-select platform icon (blue gem pool, palm trees, small ruins,
	@ magenta transparent background): composited with its real palette
	@ (gStaticData_0861BD48) and tilemap (gStaticData_0863053C, 32x32
	@ tiles) via the package at gStaticData_0816C58C.
	.incbin "build/crashbandicootxs/graphics/intro/38_62556c_8bpp_tiles.8bpp.lz"

.global gStaticData_08628C50
gStaticData_08628C50:
	@ LZ77 tile graphics (4bpp) (16192 bytes decompressed, 506 tiles). A
	@ red/fiery smoke texture: composited with its real palette
	@ (gStaticData_0861BF30) and tilemap (gStaticData_086308F0) via the
	@ package at gStaticData_0817C594.
	.incbin "build/crashbandicootxs/graphics/intro/39_628c50_tiles.4bpp.lz"

.global gStaticData_0862A958
gStaticData_0862A958:
	@ LZ77 tile graphics (4bpp) (5344 bytes decompressed, 167 tiles - a
	@ prime tile count, so stored as a 1-tile-tall strip). A fire/aura glow
	@ effect (green transparent background, orange/red/magenta outline):
	@ composited with its real palette (gStaticData_0861BF58) and tilemap
	@ (gStaticData_08630E1C) via the package at gStaticData_0817C5A8.
	@ Originally left as raw binary ("not clearly identifiable") since a
	@ bare tileset doesn't look like anything on its own without the
	@ tilemap.
	.incbin "build/crashbandicootxs/graphics/intro/40_62a958_tiles.4bpp.lz", 0, 0x9F4

.global gStaticData_0862B34C
gStaticData_0862B34C:
	@ LZ77 tile graphics (8bpp) (6272 bytes decompressed, 98 tiles). Uka
	@ Uka's mask: composited with its real palette (gStaticData_0861BF80)
	@ and tilemap (gStaticData_08631158) via the package at
	@ gStaticData_0817C5BC.
	.incbin "build/crashbandicootxs/graphics/intro/41_62b34c_8bpp_tiles.8bpp.lz"

.global gStaticData_0862C2C8
gStaticData_0862C2C8:
	@ LZ77 tile graphics (4bpp) (704 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/42_62c2c8_tiles.4bpp.lz"

.global gStaticData_0862C4B0
gStaticData_0862C4B0:
	@ LZ77 tile graphics (4bpp) (3648 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/43_62c4b0_tiles.4bpp.lz"

.global gStaticData_0862CC3C
gStaticData_0862CC3C:
	@ LZ77 compressed data (1184 bytes decompressed) - not clearly identifiable as pixel graphics
	.incbin "build/crashbandicootxs/graphics/intro/44_62cc3c.bin.lz"

.global gStaticData_0862CE70
gStaticData_0862CE70:
	@ LZ77 tile graphics (4bpp) (1088 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/45_62ce70_tiles.4bpp.lz"

.global gStaticData_0862D0CC
gStaticData_0862D0CC:
	@ LZ77 tile graphics (4bpp) (14976 bytes decompressed, 468 tiles). This
	@ is the legal/credits text screen: referenced (with palette
	@ gStaticData_0861C1FC and tilemap gStaticData_086315BC) from a graphics
	@ package struct at gUniversalLogoBg. Originally misclassified as
	@ 8bpp - 14976 divides evenly by both 32 and 64, and this only decodes
	@ as a coherent image (not noise) once composited with its real
	@ palette+tilemap as 4bpp.
	.incbin "build/crashbandicootxs/graphics/intro/46_62d0cc_tiles.4bpp.lz"

.global gStaticData_0862E3B0
gStaticData_0862E3B0:
	@ LZ77 tile graphics (8bpp) (9152 bytes decompressed, 143 tiles). This is
	@ the "Crash Bandicoot XS" title/logo art: referenced (with palette
	@ gStaticData_0861C224 and tilemap gStaticData_0863183C) from a graphics
	@ package struct at gTitleScreenBg, loaded onto BG2 by LoadTitleScreenBg.
	.incbin "build/crashbandicootxs/graphics/intro/47_62e3b0_8bpp_tiles.8bpp.lz", 0, 0x1774

.global gStaticData_0862FB24
gStaticData_0862FB24:
	@ LZ77 compressed data (1280 bytes decompressed) - the tilemap for the
	@ sky/clouds background, gStaticData_0861C30C (32x20 tiles, 16-bit
	@ entries). Originally misclassified as 4bpp tile graphics (1280 bytes
	@ divides evenly by 32, the 4bpp tile size, purely by coincidence).
	.incbin "build/crashbandicootxs/graphics/intro/48_62fb24.bin.lz"

.global gStaticData_0862FFF4
gStaticData_0862FFF4:
	@ LZ77 compressed data (1200 bytes decompressed) - the tilemap for
	@ Crash's face/warp-room background, gStaticData_0861E5F8 (30x20
	@ tiles, 16-bit entries).
	.incbin "build/crashbandicootxs/graphics/intro/49_62fff4.bin.lz", 0, 0x548

.global gStaticData_0863053C
gStaticData_0863053C:
	@ LZ77 compressed data (2048 bytes decompressed) - the tilemap for the
	@ level-select platform icon, gStaticData_0862556C (32x32 tiles,
	@ 16-bit entries). Originally misclassified as 4bpp tile graphics (2048
	@ bytes divides evenly by 32, the 4bpp tile size, purely by
	@ coincidence).
	.incbin "build/crashbandicootxs/graphics/intro/50_63053c.bin.lz"

.global gStaticData_086308F0
gStaticData_086308F0:
	@ LZ77 compressed data (1200 bytes decompressed) - the tilemap for the
	@ red/fiery smoke texture, gStaticData_08628C50 (30x20 tiles, 16-bit
	@ entries).
	.incbin "build/crashbandicootxs/graphics/intro/51_6308f0.bin.lz"

.global gStaticData_08630E1C
gStaticData_08630E1C:
	@ LZ77 compressed data (1200 bytes decompressed) - the tilemap for the
	@ fire/aura glow effect, gStaticData_0862A958 (30x20 tiles, 16-bit
	@ entries).
	.incbin "build/crashbandicootxs/graphics/intro/52_630e1c.bin.lz"

.global gStaticData_08631158
gStaticData_08631158:
	@ LZ77 compressed data (1200 bytes decompressed) - the tilemap for Uka
	@ Uka's mask, gStaticData_0862B34C (30x20 tiles, 16-bit entries).
	.incbin "build/crashbandicootxs/graphics/intro/53_631158.bin.lz"

.global gStaticData_08631374
gStaticData_08631374:
	@ LZ77 compressed data (unidentified) (48 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/54_631374.bin.lz"

.global gStaticData_086313B0
gStaticData_086313B0:
	@ LZ77 tile graphics (4bpp) (640 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/55_6313b0_tiles.4bpp.lz", 0, 0x140

.global gStaticData_086314F0
gStaticData_086314F0:
	@ LZ77 tile graphics (4bpp) (128 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/56_6314f0_tiles.4bpp.lz"

.global gStaticData_08631558
gStaticData_08631558:
	@ LZ77 tile graphics (4bpp) (128 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/57_631558_tiles.4bpp.lz"

.global gStaticData_086315BC
gStaticData_086315BC:
	@ LZ77 compressed data (1200 bytes decompressed) - the tilemap for the
	@ legal/credits text screen, gStaticData_0862D0CC (30x20 tiles, 16-bit
	@ entries).
	.incbin "build/crashbandicootxs/graphics/intro/58_6315bc.bin.lz"

.global gStaticData_0863183C
gStaticData_0863183C:
	@ LZ77 palette (256 colors) (512 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/59_63183c.gbapal.lz"

.global gStaticData_086319A0
gStaticData_086319A0:
	@ LZ77 tile graphics (4bpp) (32 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/60_6319a0_tiles.4bpp.lz", 0, 0x28

.global gStaticData_086319C8
gStaticData_086319C8:
	@ LZ77 tile graphics (4bpp) (32 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/61_6319c8_tiles.4bpp.lz", 0, 0x28

.global gStaticData_086319F0
gStaticData_086319F0:
	@ LZ77 tile graphics (4bpp) (32 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/62_6319f0_tiles.4bpp.lz", 0, 0x28

.global gStaticData_08631A18
gStaticData_08631A18:
	@ LZ77 tile graphics (4bpp) (32 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/63_631a18_tiles.4bpp.lz", 0, 0x28

.global gStaticData_08631A40
gStaticData_08631A40:
	@ LZ77 tile graphics (4bpp) (32 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/64_631a40_tiles.4bpp.lz", 0, 0x28

.global gStaticData_08631A68
gStaticData_08631A68:
	@ LZ77 tile graphics (4bpp) (32 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/65_631a68_tiles.4bpp.lz", 0, 0x28

.global gStaticData_08631A90
gStaticData_08631A90:
	@ LZ77 tile graphics (4bpp) (32 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/66_631a90_tiles.4bpp.lz", 0, 0x28

.global gStaticData_08631AB8
gStaticData_08631AB8:
	@ LZ77 tile graphics (4bpp) (32 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/67_631ab8_tiles.4bpp.lz", 0, 0x14

.global gStaticData_08631ACC
gStaticData_08631ACC:
	@ LZ77 tile graphics (4bpp) (4608 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/68_631acc_tiles.4bpp.lz"

.global gStaticData_086324B4
gStaticData_086324B4:
	@ LZ77 tile graphics (4bpp) (2048 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/69_6324b4_tiles.4bpp.lz"

.global gStaticData_08632820
gStaticData_08632820:
	@ LZ77 tile graphics (4bpp) (2048 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/70_632820_tiles.4bpp.lz"

.global gStaticData_08632BC4
gStaticData_08632BC4:
	@ LZ77 tile graphics (4bpp) (6144 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/71_632bc4_tiles.4bpp.lz"

.global gStaticData_086334C4
gStaticData_086334C4:
	@ LZ77 tile graphics (4bpp) (7680 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/72_6334c4_tiles.4bpp.lz", 0, 0xDAC

.global gStaticData_08634270
gStaticData_08634270:
	@ LZ77 tile graphics (4bpp) (25600 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/73_634270_tiles.4bpp.lz"

.global gStaticData_08636EF4
gStaticData_08636EF4:
	@ LZ77 tile graphics (4bpp) (4608 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/74_636ef4_tiles.4bpp.lz"

.global gStaticData_08637604
gStaticData_08637604:
	@ LZ77 tile graphics (4bpp) (1024 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/75_637604_tiles.4bpp.lz"

.global gStaticData_086377C0
gStaticData_086377C0:
	@ LZ77 tile graphics (4bpp) (2048 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/00_6377c0_tiles.4bpp.lz"

.global gStaticData_08637A70
gStaticData_08637A70:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gStaticData_0863CF98
	.incbin "build/crashbandicootxs/graphics/tileset1/01_637a70_8bpp_tiles.8bpp.lz"

.global gStaticData_086382C8
gStaticData_086382C8:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gStaticData_0863D01C
	.incbin "build/crashbandicootxs/graphics/tileset1/02_6382c8_8bpp_tiles.8bpp.lz", 0, 0xAA0

.global gStaticData_08638D68
gStaticData_08638D68:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gStaticData_0863D0A0
	.incbin "build/crashbandicootxs/graphics/tileset1/03_638d68_8bpp_tiles.8bpp.lz"

.global gStaticData_0863945C
gStaticData_0863945C:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gStaticData_0863D124
	.incbin "build/crashbandicootxs/graphics/tileset1/04_63945c_8bpp_tiles.8bpp.lz", 0, 0x8BC

.global gStaticData_08639D18
gStaticData_08639D18:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gStaticData_0863D1A8
	.incbin "build/crashbandicootxs/graphics/tileset1/05_639d18_8bpp_tiles.8bpp.lz"

.global gStaticData_0863A60C
gStaticData_0863A60C:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gStaticData_0863D22C
	.incbin "build/crashbandicootxs/graphics/tileset1/06_63a60c_8bpp_tiles.8bpp.lz"

.global gStaticData_0863AD90
gStaticData_0863AD90:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gStaticData_0863D2B0
	.incbin "build/crashbandicootxs/graphics/tileset1/07_63ad90_8bpp_tiles.8bpp.lz"

.global gStaticData_0863B668
gStaticData_0863B668:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gStaticData_0863D334
	.incbin "build/crashbandicootxs/graphics/tileset1/08_63b668_8bpp_tiles.8bpp.lz"

.global gStaticData_0863BDD4
gStaticData_0863BDD4:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gStaticData_0863D3B8
	.incbin "build/crashbandicootxs/graphics/tileset1/09_63bdd4_8bpp_tiles.8bpp.lz", 0, 0x810

.global gStaticData_0863C5E4
gStaticData_0863C5E4:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gStaticData_0863D43C
	.incbin "build/crashbandicootxs/graphics/tileset1/10_63c5e4_8bpp_tiles.8bpp.lz"

.global gStaticData_0863CF98
gStaticData_0863CF98:
	@ LZ77 palette (256 colors) (512 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/11_63cf98.gbapal.lz", 0, 0x84

.global gStaticData_0863D01C
gStaticData_0863D01C:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for the circular level-select icon at gStaticData_086382C8.
	@ Kept as raw binary: some entries have a stray set bit 15 that a
	@ standard .pal round-trip through gbagfx can't reproduce (RGB555 only
	@ uses bits 0-14), which broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/tileset1/12_63d01c.bin.lz", 0, 0x84

.global gStaticData_0863D0A0
gStaticData_0863D0A0:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for the circular level-select icon at gStaticData_08638D68.
	@ Kept as raw binary: some entries have a stray set bit 15 that a
	@ standard .pal round-trip through gbagfx can't reproduce (RGB555 only
	@ uses bits 0-14), which broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/tileset1/13_63d0a0.bin.lz", 0, 0x84

.global gStaticData_0863D124
gStaticData_0863D124:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for the circular level-select icon at gStaticData_0863945C.
	@ Kept as raw binary: some entries have a stray set bit 15 that a
	@ standard .pal round-trip through gbagfx can't reproduce (RGB555 only
	@ uses bits 0-14), which broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/tileset1/14_63d124.bin.lz", 0, 0x84

.global gStaticData_0863D1A8
gStaticData_0863D1A8:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for the circular level-select icon at gStaticData_08639D18.
	@ Kept as raw binary: some entries have a stray set bit 15 that a
	@ standard .pal round-trip through gbagfx can't reproduce (RGB555 only
	@ uses bits 0-14), which broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/tileset1/15_63d1a8.bin.lz", 0, 0x84

.global gStaticData_0863D22C
gStaticData_0863D22C:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for the circular level-select icon at gStaticData_0863A60C.
	@ Kept as raw binary: some entries have a stray set bit 15 that a
	@ standard .pal round-trip through gbagfx can't reproduce (RGB555 only
	@ uses bits 0-14), which broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/tileset1/16_63d22c.bin.lz", 0, 0x84

.global gStaticData_0863D2B0
gStaticData_0863D2B0:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for the circular level-select icon at gStaticData_0863AD90.
	@ Kept as raw binary: some entries have a stray set bit 15 that a
	@ standard .pal round-trip through gbagfx can't reproduce (RGB555 only
	@ uses bits 0-14), which broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/tileset1/17_63d2b0.bin.lz"

.global gStaticData_0863D334
gStaticData_0863D334:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for the circular level-select icon at gStaticData_0863B668.
	@ Kept as raw binary: some entries have a stray set bit 15 that a
	@ standard .pal round-trip through gbagfx can't reproduce (RGB555 only
	@ uses bits 0-14), which broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/tileset1/18_63d334.bin.lz", 0, 0x84

.global gStaticData_0863D3B8
gStaticData_0863D3B8:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for the circular level-select icon at gStaticData_0863BDD4.
	@ Kept as raw binary: some entries have a stray set bit 15 that a
	@ standard .pal round-trip through gbagfx can't reproduce (RGB555 only
	@ uses bits 0-14), which broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/tileset1/19_63d3b8.bin.lz", 0, 0x84

.global gStaticData_0863D43C
gStaticData_0863D43C:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for the circular level-select icon at gStaticData_0863C5E4.
	@ Kept as raw binary: some entries have a stray set bit 15 that a
	@ standard .pal round-trip through gbagfx can't reproduce (RGB555 only
	@ uses bits 0-14), which broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/tileset1/20_63d43c.bin.lz"

.global gStaticData_0863D4C0
gStaticData_0863D4C0:
	@ LZ77 tile graphics (4bpp) (23520 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/21_63d4c0_tiles.4bpp.lz"

.global gStaticData_08640EF8
gStaticData_08640EF8:
	@ LZ77 tile graphics (4bpp) (23392 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/22_640ef8_tiles.4bpp.lz"

.global gStaticData_08644A88
gStaticData_08644A88:
	@ LZ77 compressed data (24352 bytes decompressed) - not clearly identifiable as pixel graphics
	.incbin "build/crashbandicootxs/graphics/tileset1/23_644a88.bin.lz"

.global gStaticData_08649418
gStaticData_08649418:
	@ LZ77 compressed data (15968 bytes decompressed) - not clearly identifiable as pixel graphics
	.incbin "build/crashbandicootxs/graphics/tileset1/24_649418.bin.lz", 0, 0x2744

.global gStaticData_0864BB5C
gStaticData_0864BB5C:
	@ LZ77 compressed data (12576 bytes decompressed) - not clearly identifiable as pixel graphics
	.incbin "build/crashbandicootxs/graphics/tileset1/25_64bb5c.bin.lz"

.global gStaticData_0864D7E8
gStaticData_0864D7E8:
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

@ gStaticData_087E3BEC..gLogoActorVtable: src/data/entity_vtables_7e3bec.c

@ 0x087E55E4..0x087E5FCC: the IWRAM image (asm/intr_main.s, src/iwram/),
@ linked to run at 0x03000000 - the `iwram` section in ldscript.txt.
@ 0x087E5FCC..0x08800000: 0xFF cartridge fill, the `rom_fill` section.

