.section .rodata

@ gStaticData_0803B8B0..gStaticData_080B1444: src/data/cell_anim_03b8b0.c

.section .rodata.080B2120

.global gStaticData_080B2120
gStaticData_080B2120:
	@ LZ77 compressed data (unidentified) (213064 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/unknown/00_0b2120.bin.lz", 0, 0xEB16

.global gStaticData_080C0C36
gStaticData_080C0C36:
	@ zero padding after the LZ77 stream, up to the next word-aligned table
	.incbin "baserom.gba", 0x000C0C36, 0x00000002

@ gStaticData_080C0C38..gStaticData_080C19F0: src/data/sub_effect_0c0c38.c

@ gStaticData_080C2758..gStaticData_080DA1D8: src/data/rle_sprites_0c2758.c

@ gStaticData_080FF1B0..gStaticData_08140DCC: src/data/cell_anim_0ff1b0.c

.section .rodata.0814174C

.global gStaticData_0814174C
gStaticData_0814174C:
	@ LZ77 compressed data (unidentified) (207124 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/unknown/01_14174c.bin.lz", 0, 0x10376

.global gStaticData_08151AC2
gStaticData_08151AC2:
	@ zero padding after the LZ77 stream, up to the next word-aligned table
	.incbin "baserom.gba", 0x00151AC2, 0x00000002

@ gStaticData_08151AC4..gStaticData_0815A030: src/data/bg_picture_151ac4.c

@ gStaticData_0815A050: src/data/rle_sprites_15a050.c

.section .rodata.08167AD4

.global gStaticData_08167AD4
gStaticData_08167AD4:
	.incbin "baserom.gba", 0x00167AD4, 0x00000200

.global gStaticData_08167CD4
gStaticData_08167CD4:
	.incbin "baserom.gba", 0x00167CD4, 0x00001E14

.global gStaticData_08169AE8
gStaticData_08169AE8:
	.incbin "baserom.gba", 0x00169AE8, 0x00000200

.global gStaticData_08169CE8
gStaticData_08169CE8:
	.incbin "baserom.gba", 0x00169CE8, 0x00000B28

.global gStaticData_0816A810
gStaticData_0816A810:
	.incbin "baserom.gba", 0x0016A810, 0x00000010

.global gStaticData_0816A820
gStaticData_0816A820:
	.incbin "baserom.gba", 0x0016A820, 0x00000200

@ gStaticData_0816AA20: src/data/song_table_16aa20.c

.section .rodata.0816AA6C

.global gStaticData_0816AA6C
gStaticData_0816AA6C:
	@ GAX2 sound-effect trigger table: 99 entries of {slot_id, pitch_offset,
	@ volume}, indexed by the sound effect IDs passed to PlaySfx. Reuses
	@ the same instrument/sample pool as the music (sound/gax_manifest.json)
	@ instead of storing separate sound-effect audio.
	@
	@ Built from sound/sfx_table.json by tools/sfx_table.py.
	.incbin "build/crashbandicootxs/sound/sfx_table.bin"

.global gStaticData_0816AF10
gStaticData_0816AF10:
	.incbin "baserom.gba", 0x0016AF10, 0x00000228

.global gStaticData_0816B138
gStaticData_0816B138:
	.incbin "baserom.gba", 0x0016B138, 0x00000002

.global gStaticData_0816B13A
gStaticData_0816B13A:
	.incbin "baserom.gba", 0x0016B13A, 0x00000020

.global gStaticData_0816B15A
gStaticData_0816B15A:
	.incbin "baserom.gba", 0x0016B15A, 0x00000020

.global gStaticData_0816B17A
gStaticData_0816B17A:
	.incbin "baserom.gba", 0x0016B17A, 0x00000020

.global gStaticData_0816B19A
gStaticData_0816B19A:
	.incbin "baserom.gba", 0x0016B19A, 0x00000022

.global gStaticData_0816B1BC
gStaticData_0816B1BC:
	.incbin "baserom.gba", 0x0016B1BC, 0x00000014

.global gStaticData_0816B1D0
gStaticData_0816B1D0:
	.incbin "baserom.gba", 0x0016B1D0, 0x00000014

.global gStaticData_0816B1E4
gStaticData_0816B1E4:
	.incbin "baserom.gba", 0x0016B1E4, 0x00000008

.global gStaticData_0816B1EC
gStaticData_0816B1EC:
	.incbin "baserom.gba", 0x0016B1EC, 0x00000020

.global gStaticData_0816B20C
gStaticData_0816B20C:
	.incbin "baserom.gba", 0x0016B20C, 0x00000010

.global gStaticData_0816B21C
gStaticData_0816B21C:
	.incbin "baserom.gba", 0x0016B21C, 0x00000028

.global gStaticData_0816B244
gStaticData_0816B244:
	.incbin "baserom.gba", 0x0016B244, 0x00000014

.global gStaticData_0816B258
gStaticData_0816B258:
	.incbin "baserom.gba", 0x0016B258, 0x00000018

.global gStaticData_0816B270
gStaticData_0816B270:
	.incbin "baserom.gba", 0x0016B270, 0x0000000C

.global gStaticData_0816B27C
gStaticData_0816B27C:
	.incbin "baserom.gba", 0x0016B27C, 0x00000008

@ gStaticData_0816B284: src/data/bg_package_16b284.c

.section .rodata.0816B298

.global gStaticData_0816B298
gStaticData_0816B298:
	.incbin "baserom.gba", 0x0016B298, 0x00000028

.global gStaticData_0816B2C0
gStaticData_0816B2C0:
	.incbin "baserom.gba", 0x0016B2C0, 0x00000020

.global gStaticData_0816B2E0
gStaticData_0816B2E0:
	.incbin "baserom.gba", 0x0016B2E0, 0x0000000C

.global gStaticData_0816B2EC
gStaticData_0816B2EC:
	.incbin "baserom.gba", 0x0016B2EC, 0x0000000C

.global gStaticData_0816B2F8
gStaticData_0816B2F8:
	.incbin "baserom.gba", 0x0016B2F8, 0x00000008

.global gStaticData_0816B300
gStaticData_0816B300:
	.incbin "baserom.gba", 0x0016B300, 0x00000004

.global gStaticData_0816B304
gStaticData_0816B304:
	.incbin "baserom.gba", 0x0016B304, 0x00000318

.global gStaticData_0816B61C
gStaticData_0816B61C:
	.incbin "baserom.gba", 0x0016B61C, 0x000002A4

.global gStaticData_0816B8C0
gStaticData_0816B8C0:
	.incbin "baserom.gba", 0x0016B8C0, 0x0000006C

@ gStaticData_0816B92C..gStaticData_0816B934: src/data/entry_set_16b92c.c

.section .rodata.0816B93C

.global gStaticData_0816B93C
gStaticData_0816B93C:
	.incbin "baserom.gba", 0x0016B93C, 0x00000050

.global gStaticData_0816B98C
gStaticData_0816B98C:
	.incbin "baserom.gba", 0x0016B98C, 0x00000020

.global gStaticData_0816B9AC
gStaticData_0816B9AC:
	.incbin "baserom.gba", 0x0016B9AC, 0x00000020

.global gStaticData_0816B9CC
gStaticData_0816B9CC:
	.incbin "baserom.gba", 0x0016B9CC, 0x00000020

.global gStaticData_0816B9EC
gStaticData_0816B9EC:
	.incbin "baserom.gba", 0x0016B9EC, 0x00000020

.global gStaticData_0816BA0C
gStaticData_0816BA0C:
	.incbin "baserom.gba", 0x0016BA0C, 0x00000020

.global gStaticData_0816BA2C
gStaticData_0816BA2C:
	.incbin "baserom.gba", 0x0016BA2C, 0x00000020

.global gStaticData_0816BA4C
gStaticData_0816BA4C:
	.incbin "baserom.gba", 0x0016BA4C, 0x00000020

.global gStaticData_0816BA6C
gStaticData_0816BA6C:
	.incbin "baserom.gba", 0x0016BA6C, 0x00000020

.global gStaticData_0816BA8C
gStaticData_0816BA8C:
	.incbin "baserom.gba", 0x0016BA8C, 0x00000020

.global gStaticData_0816BAAC
gStaticData_0816BAAC:
	.incbin "baserom.gba", 0x0016BAAC, 0x00000020

.global gStaticData_0816BACC
gStaticData_0816BACC:
	.incbin "baserom.gba", 0x0016BACC, 0x00000020

.global gStaticData_0816BAEC
gStaticData_0816BAEC:
	.incbin "baserom.gba", 0x0016BAEC, 0x00000020

.global gStaticData_0816BB0C
gStaticData_0816BB0C:
	.incbin "baserom.gba", 0x0016BB0C, 0x00000020

.global gStaticData_0816BB2C
gStaticData_0816BB2C:
	.incbin "baserom.gba", 0x0016BB2C, 0x00000020

.global gStaticData_0816BB4C
gStaticData_0816BB4C:
	.incbin "baserom.gba", 0x0016BB4C, 0x00000020

.global gStaticData_0816BB6C
gStaticData_0816BB6C:
	.incbin "baserom.gba", 0x0016BB6C, 0x00000028

.global gStaticData_0816BB94
gStaticData_0816BB94:
	.incbin "baserom.gba", 0x0016BB94, 0x00000004

.global gStaticData_0816BB98
gStaticData_0816BB98:
	.incbin "baserom.gba", 0x0016BB98, 0x00000016

.global gStaticData_0816BBAE
gStaticData_0816BBAE:
	.incbin "baserom.gba", 0x0016BBAE, 0x00000016

.global gStaticData_0816BBC4
gStaticData_0816BBC4:
	.incbin "baserom.gba", 0x0016BBC4, 0x00000016

.global gStaticData_0816BBDA
gStaticData_0816BBDA:
	.incbin "baserom.gba", 0x0016BBDA, 0x00000016

.global gStaticData_0816BBF0
gStaticData_0816BBF0:
	.incbin "baserom.gba", 0x0016BBF0, 0x000000A8

.global gStaticData_0816BC98
gStaticData_0816BC98:
	.incbin "baserom.gba", 0x0016BC98, 0x00000268

.global gStaticData_0816BF00
gStaticData_0816BF00:
	.incbin "baserom.gba", 0x0016BF00, 0x00000008

.global gStaticData_0816BF08
gStaticData_0816BF08:
	.incbin "baserom.gba", 0x0016BF08, 0x0000000C

.global gStaticData_0816BF14
gStaticData_0816BF14:
	.incbin "baserom.gba", 0x0016BF14, 0x0000000C

@ gStaticData_0816BF20..gStaticData_0816C070: src/data/action_table_16bf20.c

.section .rodata.0816C090

.global gStaticData_0816C090
gStaticData_0816C090:
	.incbin "baserom.gba", 0x0016C090, 0x000001C0

@ gStaticData_0816C250..gStaticData_0816C2D0: src/data/player_pmf_16c250.c

.section .rodata.0816C2D8

.global gStaticData_0816C2D8
gStaticData_0816C2D8:
	.incbin "baserom.gba", 0x0016C2D8, 0x00000030

.global gStaticData_0816C308
gStaticData_0816C308:
	.incbin "baserom.gba", 0x0016C308, 0x00000003

.global gStaticData_0816C30B
gStaticData_0816C30B:
	.incbin "baserom.gba", 0x0016C30B, 0x0000004D

.global gStaticData_0816C358
gStaticData_0816C358:
	.incbin "baserom.gba", 0x0016C358, 0x00000004

.global gStaticData_0816C35C
gStaticData_0816C35C:
	.incbin "baserom.gba", 0x0016C35C, 0x00000003

.global gStaticData_0816C35F
gStaticData_0816C35F:
	.incbin "baserom.gba", 0x0016C35F, 0x00000003

.global gStaticData_0816C362
gStaticData_0816C362:
	.incbin "baserom.gba", 0x0016C362, 0x00000006

.global gStaticData_0816C368
gStaticData_0816C368:
	.incbin "baserom.gba", 0x0016C368, 0x00000010

.global gStaticData_0816C378
gStaticData_0816C378:
	.incbin "baserom.gba", 0x0016C378, 0x00000018

.global gStaticData_0816C390
gStaticData_0816C390:
	.incbin "baserom.gba", 0x0016C390, 0x00000010

.global gStaticData_0816C3A0
gStaticData_0816C3A0:
	.incbin "baserom.gba", 0x0016C3A0, 0x00000018

.global gStaticData_0816C3B8
gStaticData_0816C3B8:
	.incbin "baserom.gba", 0x0016C3B8, 0x00000030

.global gStaticData_0816C3E8
gStaticData_0816C3E8:
	.incbin "baserom.gba", 0x0016C3E8, 0x0000000C

.global gStaticData_0816C3F4
gStaticData_0816C3F4:
	.incbin "baserom.gba", 0x0016C3F4, 0x00000024

@ gStaticData_0816C418..gStaticData_0816C458: src/data/entry_set_16c418.c

.section .rodata.0816C460

.global gStaticData_0816C460
gStaticData_0816C460:
	.incbin "baserom.gba", 0x0016C460, 0x00000024

@ gStaticData_0816C484: src/data/bg_package_16c484.c

.section .rodata.0816C498

.global gStaticData_0816C498
gStaticData_0816C498:
	.incbin "baserom.gba", 0x0016C498, 0x00000008

.global gStaticData_0816C4A0
gStaticData_0816C4A0:
	.incbin "baserom.gba", 0x0016C4A0, 0x00000008

.global gStaticData_0816C4A8
gStaticData_0816C4A8:
	.incbin "baserom.gba", 0x0016C4A8, 0x00000008

.global gStaticData_0816C4B0
gStaticData_0816C4B0:
	.incbin "baserom.gba", 0x0016C4B0, 0x00000008

.global gStaticData_0816C4B8
gStaticData_0816C4B8:
	.incbin "baserom.gba", 0x0016C4B8, 0x00000008

.global gStaticData_0816C4C0
gStaticData_0816C4C0:
	.incbin "baserom.gba", 0x0016C4C0, 0x00000008

.global gStaticData_0816C4C8
gStaticData_0816C4C8:
	.incbin "baserom.gba", 0x0016C4C8, 0x00000008

.global gStaticData_0816C4D0
gStaticData_0816C4D0:
	.incbin "baserom.gba", 0x0016C4D0, 0x00000008

.global gStaticData_0816C4D8
gStaticData_0816C4D8:
	.incbin "baserom.gba", 0x0016C4D8, 0x00000030

.global gStaticData_0816C508
gStaticData_0816C508:
	.incbin "baserom.gba", 0x0016C508, 0x00000030

.global gStaticData_0816C538
gStaticData_0816C538:
	.incbin "baserom.gba", 0x0016C538, 0x00000010

.global gStaticData_0816C548
gStaticData_0816C548:
	.incbin "baserom.gba", 0x0016C548, 0x00000010

.global gStaticData_0816C558
gStaticData_0816C558:
	.incbin "baserom.gba", 0x0016C558, 0x00000014

.global gStaticData_0816C56C
gStaticData_0816C56C:
	.incbin "baserom.gba", 0x0016C56C, 0x00000020

@ gStaticData_0816C58C: src/data/bg_package_16c58c.c

.section .rodata.0816C5F0

.global gStaticData_0816C5F0
gStaticData_0816C5F0:
	.incbin "baserom.gba", 0x0016C5F0, 0x00000020

.global gStaticData_0816C610
gStaticData_0816C610:
	.incbin "baserom.gba", 0x0016C610, 0x00000014

.global gStaticData_0816C624
gStaticData_0816C624:
	.incbin "baserom.gba", 0x0016C624, 0x00000010

.global gStaticData_0816C634
gStaticData_0816C634:
	.incbin "baserom.gba", 0x0016C634, 0x00000010

.global gStaticData_0816C644
gStaticData_0816C644:
	.incbin "baserom.gba", 0x0016C644, 0x00000030

.global gStaticData_0816C674
gStaticData_0816C674:
	.incbin "baserom.gba", 0x0016C674, 0x00000030

@ gStaticData_0816C6A4: src/data/dispatch_table_16c6a4.c

@ gStaticData_0816C814..gStaticData_0816CD80: src/data/level_table_16c814.c

@ gStaticData_0816D1C8..gStaticData_0816D1F4: src/data/cutscenes_16d1c8.c

@ gStaticData_081725A8..0x08172CD4 (terrain types): src/data/terrain_1725a8.c

@ 0x08172CD4..0x08174BE0 (the UI text of six languages): src/data/ui_text_172cd4.c

@ gStaticData_08174BE0..gStaticData_081751D4: src/data/hud_fonts_174be0.c

@ gStaticData_08175558..gStaticData_081756C4: src/data/actor_category_175558.c

.section .rodata.08175760

.global gStaticData_08175760
gStaticData_08175760:
	.incbin "baserom.gba", 0x00175760, 0x00003800

.global gStaticData_08178F60
gStaticData_08178F60:
	.incbin "baserom.gba", 0x00178F60, 0x00000010

.global gStaticData_08178F70
gStaticData_08178F70:
	.incbin "baserom.gba", 0x00178F70, 0x00000010

.global gStaticData_08178F80
gStaticData_08178F80:
	.incbin "baserom.gba", 0x00178F80, 0x00001738

@ gStaticData_0817A6B8: src/data/actor_pmf_17a6b8.c

.section .rodata.0817A728

.global gStaticData_0817A728
gStaticData_0817A728:
	.incbin "baserom.gba", 0x0017A728, 0x00000020

.global gStaticData_0817A748
gStaticData_0817A748:
	.incbin "baserom.gba", 0x0017A748, 0x00000020

.global gStaticData_0817A768
gStaticData_0817A768:
	.incbin "baserom.gba", 0x0017A768, 0x0000000C

.global gStaticData_0817A774
gStaticData_0817A774:
	.incbin "baserom.gba", 0x0017A774, 0x0000000C

.global gStaticData_0817A780
gStaticData_0817A780:
	.incbin "baserom.gba", 0x0017A780, 0x0000000C

.global gStaticData_0817A78C
gStaticData_0817A78C:
	.incbin "baserom.gba", 0x0017A78C, 0x0000000C

.global gStaticData_0817A798
gStaticData_0817A798:
	.incbin "baserom.gba", 0x0017A798, 0x00000020

.global gStaticData_0817A7B8
gStaticData_0817A7B8:
	.incbin "baserom.gba", 0x0017A7B8, 0x00000020

.global gStaticData_0817A7D8
gStaticData_0817A7D8:
	.incbin "baserom.gba", 0x0017A7D8, 0x00000020

.global gStaticData_0817A7F8
gStaticData_0817A7F8:
	.incbin "baserom.gba", 0x0017A7F8, 0x00000048

@ gStaticData_0817A840: src/data/actor_state_fn_17a840.c

.section .rodata.0817A850

.global gStaticData_0817A850
gStaticData_0817A850:
	.incbin "baserom.gba", 0x0017A850, 0x00000030

@ gStaticData_0817A880: src/data/frame_table_17a880.c

.section .rodata.0817AA6C

.global gStaticData_0817AA6C
gStaticData_0817AA6C:
	.incbin "baserom.gba", 0x0017AA6C, 0x00000020

.global gStaticData_0817AA8C
gStaticData_0817AA8C:
	.incbin "baserom.gba", 0x0017AA8C, 0x0000000C

.global gStaticData_0817AA98
gStaticData_0817AA98:
	.incbin "baserom.gba", 0x0017AA98, 0x00001728

@ gStaticData_0817C1C0: src/data/actor_pmf_17c1c0.c

.section .rodata.0817C200

.global gStaticData_0817C200
gStaticData_0817C200:
	.incbin "baserom.gba", 0x0017C200, 0x00000060

@ gStaticData_0817C260..gStaticData_0817C2B8: src/data/actor_pmf_17c260.c

.section .rodata.0817C2D0

.global gStaticData_0817C2D0
gStaticData_0817C2D0:
	.incbin "baserom.gba", 0x0017C2D0, 0x000000A8

.global gStaticData_0817C378
gStaticData_0817C378:
	.incbin "baserom.gba", 0x0017C378, 0x00000060

.global gStaticData_0817C3D8
gStaticData_0817C3D8:
	.incbin "baserom.gba", 0x0017C3D8, 0x0000000C

.global gStaticData_0817C3E4
gStaticData_0817C3E4:
	.incbin "baserom.gba", 0x0017C3E4, 0x00000018

@ gStaticData_0817C3FC..gStaticData_0817C42C: src/data/actor_state_17c3fc.c

.section .rodata.0817C444

.global gStaticData_0817C444
gStaticData_0817C444:
	.incbin "baserom.gba", 0x0017C444, 0x0000000C

@ gStaticData_0817C450: src/data/actor_pmf_17c450.c

.section .rodata.0817C460

.global gStaticData_0817C460
gStaticData_0817C460:
	.incbin "baserom.gba", 0x0017C460, 0x00000050

.global gStaticData_0817C4B0
gStaticData_0817C4B0:
	.incbin "baserom.gba", 0x0017C4B0, 0x0000000C

.global gStaticData_0817C4BC
gStaticData_0817C4BC:
	.incbin "baserom.gba", 0x0017C4BC, 0x0000000C

@ gStaticData_0817C4C8..gStaticData_0817C4F8: src/data/actor_state_17c4c8.c

.section .rodata.0817C510

.global gStaticData_0817C510
gStaticData_0817C510:
	.incbin "baserom.gba", 0x0017C510, 0x00000002

.global gStaticData_0817C512
gStaticData_0817C512:
	.incbin "baserom.gba", 0x0017C512, 0x00000020

.global gStaticData_0817C532
gStaticData_0817C532:
	.incbin "baserom.gba", 0x0017C532, 0x00000020

.global gStaticData_0817C552
gStaticData_0817C552:
	.incbin "baserom.gba", 0x0017C552, 0x00000020

.global gStaticData_0817C572
gStaticData_0817C572:
	.incbin "baserom.gba", 0x0017C572, 0x00000022

@ gStaticData_0817C594..gStaticData_0817C5BC: src/data/bg_package_17c594.c

@ gStaticData_0817C5D0..gStaticData_0817CF3C: src/data/credits_17c5d0.c

@ gStaticData_0817CF40..gStaticData_0817CFA4: src/data/popup_glyphs_17cf40.c

@ gStaticData_0817CFF4..gStaticData_0817D698: src/data/level_gfx_17cff4.c

@ gStaticData_0817D6C0..gStaticData_0817D790: src/data/slot_seeds_17d6c0.c

@ gStaticData_0817D7A4..0x0817E714: src/data/countdown_17d7a4.c

@ gStaticData_0817E714: src/data/digit_glyphs_17e714.c

@ gStaticData_0817E72C..gStaticData_0817E76C: src/data/palettes_17e72c.c

@ gStaticData_0817E78C..gStaticData_08200DF4: src/data/level_tilesets_17e78c.c

@ 0x0824B638..0x08270F08 (33 rooms' level data): src/data/level_rooms_24b638.c
@ gStaticData_08270F08..gStaticData_08299DCC: src/data/level_tilesets_270f08.c
@ 0x082B91D0..0x082BF120 (8 rooms' level data): src/data/level_rooms_2b91d0.c
@ gStaticData_082BF120..gStaticData_084A4660: src/data/sprite_tiles_2bf120.c

@ gStaticData_084A5600..gSpriteBank55: src/data/sprite_banks_4a5600.c,
@ sprite_banks_4b0ae0.c, sprite_banks_4b414c.c, sprite_banks_4b9d7c.c

.section .rodata.084C0006

.global gStaticData_084C0006
gStaticData_084C0006:
	@ Shin'en GAX2 sound-effect data set: 2 bytes of alignment, 88
	@ instruments, 87 8-bit samples, the sample table, and the one handler
	@ type every sound-effect voice uses (its song data points at the
	@ instrument and sample tables). PlaySfx's instrument ids index it.
	@
	@ Built from sound/gax_sfx_manifest.json and sound/sfx_samples/*.wav by
	@ tools/gax_audio.py --sfx; see docs/audio.md.
	.incbin "build/crashbandicootxs/sound/gax_sfx_data.bin"

.global gStaticData_0855BCB4
gStaticData_0855BCB4:
	@ Starts with GaxSongHeader.sfxTypes (sub_80017BC): 9 pointers to the
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

.global gStaticData_085A4C5C
gStaticData_085A4C5C:
	@ The engine's default handler layout (sub_8038538, sub_8037FC0): the
	@ song struct of a silent one-channel song whose other objects end the
	@ music block. Built with it by tools/gax_audio.py (manifest
	@ `default_song`); see docs/audio.md.
	.incbin "build/crashbandicootxs/sound/gax_default_layout.bin"

.global gStaticData_085A4C70
gStaticData_085A4C70:
	.incbin "baserom.gba", 0x005A4C70, 0x00000100

.global gStaticData_085A4D70
gStaticData_085A4D70:
	.incbin "baserom.gba", 0x005A4D70, 0x00000100

.global gStaticData_085A4E70
gStaticData_085A4E70:
	@ LZ77 tile graphics (4bpp) (5056 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/00_5a4e70_tiles.4bpp.lz", 0, 0x6A9

.global gStaticData_085A5519
gStaticData_085A5519:
	@ padding/unidentified data
	.incbin "baserom.gba", 0x005A5519, 0x00000003

.global gStaticData_085A551C
gStaticData_085A551C:
	@ LZ77 tile graphics (4bpp) (9600 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/00_5a551c_tiles.4bpp.lz", 0, 0xBE3

.global gStaticData_085A60FF
gStaticData_085A60FF:
	@ padding/unidentified data
	.incbin "baserom.gba", 0x005A60FF, 0x0000004D

.global gStaticData_085A614C
gStaticData_085A614C:
	.incbin "baserom.gba", 0x005A614C, 0x00000004

.global gStaticData_085A6150
gStaticData_085A6150:
	.incbin "baserom.gba", 0x005A6150, 0x00000060

.global gStaticData_085A61B0
gStaticData_085A61B0:
	.incbin "baserom.gba", 0x005A61B0, 0x0000000C

.global gStaticData_085A61BC
gStaticData_085A61BC:
	.incbin "baserom.gba", 0x005A61BC, 0x00000014

.global gStaticData_085A61D0
gStaticData_085A61D0:
	.incbin "baserom.gba", 0x005A61D0, 0x0000000C

.global gStaticData_085A61DC
gStaticData_085A61DC:
	.incbin "baserom.gba", 0x005A61DC, 0x00000010

.global gStaticData_085A61EC
gStaticData_085A61EC:
	.incbin "baserom.gba", 0x005A61EC, 0x0000000C

.global gStaticData_085A61F8
gStaticData_085A61F8:
	.incbin "baserom.gba", 0x005A61F8, 0x0000001C

.global gStaticData_085A6214
gStaticData_085A6214:
	.incbin "baserom.gba", 0x005A6214, 0x00000008

.global gStaticData_085A621C
gStaticData_085A621C:
	.incbin "baserom.gba", 0x005A621C, 0x000000AC

.global gStaticData_085A62C8
gStaticData_085A62C8:
	.incbin "baserom.gba", 0x005A62C8, 0x00000004

.global gStaticData_085A62CC
gStaticData_085A62CC:
	.incbin "baserom.gba", 0x005A62CC, 0x00000010

.global gStaticData_085A62DC
gStaticData_085A62DC:
	.incbin "baserom.gba", 0x005A62DC, 0x00003BD0

.global gStaticData_085A9EAC
gStaticData_085A9EAC:
	.incbin "baserom.gba", 0x005A9EAC, 0x0000004C

.global gStaticData_085A9EF8
gStaticData_085A9EF8:
	.incbin "baserom.gba", 0x005A9EF8, 0x0000000C

.global gStaticData_085A9F04
gStaticData_085A9F04:
	.incbin "baserom.gba", 0x005A9F04, 0x0000000C

.global gStaticData_085A9F10
gStaticData_085A9F10:
	.incbin "baserom.gba", 0x005A9F10, 0x00000060

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
	.incbin "build/crashbandicootxs/graphics/intro/26_61bd48.bin.lz", 0, 0x1E6

gStaticData_0861BF2E:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0061BF2E, 0x00000002

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
	.incbin "build/crashbandicootxs/graphics/intro/30_61c15c_tiles.4bpp.lz", 0, 0x26

gStaticData_0861C182:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0061C182, 0x00000002

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
	@ gStaticData_0817D7A4's (the legal/credits text screen) palette - see
	@ gStaticData_0862D0CC below.
	.incbin "build/crashbandicootxs/graphics/intro/34_61c1fc.gbapal.lz", 0, 0x28

.global gStaticData_0861C224
gStaticData_0861C224:
	@ LZ77 compressed data (512 bytes decompressed) - a 256-color RGB555
	@ palette, CONFIRMED: this is the palette for gStaticData_0862E3B0 (the
	@ "Crash Bandicoot XS" title/logo tile graphics), referenced together
	@ via the graphics package struct at gStaticData_0817D0E4. Kept as raw
	@ binary rather than .pal: some entries have a stray set bit 15 that a
	@ standard .pal round-trip through gbagfx can't reproduce (RGB555 only
	@ uses bits 0-14), which broke byte-exact rebuilding when tried as .pal
	.incbin "build/crashbandicootxs/graphics/intro/35_61c224.bin.lz", 0, 0xE5

gStaticData_0861C309:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0061C309, 0x00000003

.global gStaticData_0861C30C
gStaticData_0861C30C:
	@ LZ77 tile graphics (4bpp) (16288 bytes decompressed, 509 tiles - a
	@ prime tile count, so stored as a 1-tile-tall strip). A sky/clouds
	@ background: composited with its real palette (gStaticData_0861BADC)
	@ and tilemap (gStaticData_0862FB24, 32x20 tiles) via the package at
	@ gStaticData_0816C484. Originally left as raw binary ("not clearly
	@ identifiable") since a bare tileset doesn't look like anything on its
	@ own without the tilemap.
	.incbin "build/crashbandicootxs/graphics/intro/36_61c30c_tiles.4bpp.lz", 0, 0x22EA

gStaticData_0861E5F6:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0061E5F6, 0x00000002

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
	.incbin "build/crashbandicootxs/graphics/intro/37_61e5f8_8bpp_tiles.8bpp.lz", 0, 0x6F73

gStaticData_0862556B:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0062556B, 0x00000001

.global gStaticData_0862556C
gStaticData_0862556C:
	@ LZ77 tile graphics (8bpp) (20288 bytes decompressed, 317 tiles). A
	@ level-select platform icon (blue gem pool, palm trees, small ruins,
	@ magenta transparent background): composited with its real palette
	@ (gStaticData_0861BD48) and tilemap (gStaticData_0863053C, 32x32
	@ tiles) via the package at gStaticData_0816C58C.
	.incbin "build/crashbandicootxs/graphics/intro/38_62556c_8bpp_tiles.8bpp.lz", 0, 0x36E3

gStaticData_08628C4F:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00628C4F, 0x00000001

.global gStaticData_08628C50
gStaticData_08628C50:
	@ LZ77 tile graphics (4bpp) (16192 bytes decompressed, 506 tiles). A
	@ red/fiery smoke texture: composited with its real palette
	@ (gStaticData_0861BF30) and tilemap (gStaticData_086308F0) via the
	@ package at gStaticData_0817C594.
	.incbin "build/crashbandicootxs/graphics/intro/39_628c50_tiles.4bpp.lz", 0, 0x1D07

gStaticData_0862A957:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0062A957, 0x00000001

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
	.incbin "build/crashbandicootxs/graphics/intro/41_62b34c_8bpp_tiles.8bpp.lz", 0, 0xF79

gStaticData_0862C2C5:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0062C2C5, 0x00000003

.global gStaticData_0862C2C8
gStaticData_0862C2C8:
	@ LZ77 tile graphics (4bpp) (704 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/42_62c2c8_tiles.4bpp.lz", 0, 0x1E5

gStaticData_0862C4AD:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0062C4AD, 0x00000003

.global gStaticData_0862C4B0
gStaticData_0862C4B0:
	@ LZ77 tile graphics (4bpp) (3648 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/43_62c4b0_tiles.4bpp.lz", 0, 0x78B

gStaticData_0862CC3B:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0062CC3B, 0x00000001

.global gStaticData_0862CC3C
gStaticData_0862CC3C:
	@ LZ77 compressed data (1184 bytes decompressed) - not clearly identifiable as pixel graphics
	.incbin "build/crashbandicootxs/graphics/intro/44_62cc3c.bin.lz", 0, 0x232

gStaticData_0862CE6E:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0062CE6E, 0x00000002

.global gStaticData_0862CE70
gStaticData_0862CE70:
	@ LZ77 tile graphics (4bpp) (1088 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/45_62ce70_tiles.4bpp.lz", 0, 0x25B

gStaticData_0862D0CB:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0062D0CB, 0x00000001

.global gStaticData_0862D0CC
gStaticData_0862D0CC:
	@ LZ77 tile graphics (4bpp) (14976 bytes decompressed, 468 tiles). This
	@ is the legal/credits text screen: referenced (with palette
	@ gStaticData_0861C1FC and tilemap gStaticData_086315BC) from a graphics
	@ package struct at gStaticData_0817D7A4. Originally misclassified as
	@ 8bpp - 14976 divides evenly by both 32 and 64, and this only decodes
	@ as a coherent image (not noise) once composited with its real
	@ palette+tilemap as 4bpp.
	.incbin "build/crashbandicootxs/graphics/intro/46_62d0cc_tiles.4bpp.lz", 0, 0x12E3

gStaticData_0862E3AF:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0062E3AF, 0x00000001

.global gStaticData_0862E3B0
gStaticData_0862E3B0:
	@ LZ77 tile graphics (8bpp) (9152 bytes decompressed, 143 tiles). This is
	@ the "Crash Bandicoot XS" title/logo art: referenced (with palette
	@ gStaticData_0861C224 and tilemap gStaticData_0863183C) from a graphics
	@ package struct at gStaticData_0817D0E4, loaded onto BG2 by LoadBg2Background.
	.incbin "build/crashbandicootxs/graphics/intro/47_62e3b0_8bpp_tiles.8bpp.lz", 0, 0x1774

.global gStaticData_0862FB24
gStaticData_0862FB24:
	@ LZ77 compressed data (1280 bytes decompressed) - the tilemap for the
	@ sky/clouds background, gStaticData_0861C30C (32x20 tiles, 16-bit
	@ entries). Originally misclassified as 4bpp tile graphics (1280 bytes
	@ divides evenly by 32, the 4bpp tile size, purely by coincidence).
	.incbin "build/crashbandicootxs/graphics/intro/48_62fb24.bin.lz", 0, 0x4CF

gStaticData_0862FFF3:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0062FFF3, 0x00000001

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
	.incbin "build/crashbandicootxs/graphics/intro/50_63053c.bin.lz", 0, 0x3B3

gStaticData_086308EF:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006308EF, 0x00000001

.global gStaticData_086308F0
gStaticData_086308F0:
	@ LZ77 compressed data (1200 bytes decompressed) - the tilemap for the
	@ red/fiery smoke texture, gStaticData_08628C50 (30x20 tiles, 16-bit
	@ entries).
	.incbin "build/crashbandicootxs/graphics/intro/51_6308f0.bin.lz", 0, 0x529

gStaticData_08630E19:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00630E19, 0x00000003

.global gStaticData_08630E1C
gStaticData_08630E1C:
	@ LZ77 compressed data (1200 bytes decompressed) - the tilemap for the
	@ fire/aura glow effect, gStaticData_0862A958 (30x20 tiles, 16-bit
	@ entries).
	.incbin "build/crashbandicootxs/graphics/intro/52_630e1c.bin.lz", 0, 0x339

gStaticData_08631155:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00631155, 0x00000003

.global gStaticData_08631158
gStaticData_08631158:
	@ LZ77 compressed data (1200 bytes decompressed) - the tilemap for Uka
	@ Uka's mask, gStaticData_0862B34C (30x20 tiles, 16-bit entries).
	.incbin "build/crashbandicootxs/graphics/intro/53_631158.bin.lz", 0, 0x21A

gStaticData_08631372:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00631372, 0x00000002

.global gStaticData_08631374
gStaticData_08631374:
	@ LZ77 compressed data (unidentified) (48 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/54_631374.bin.lz", 0, 0x39

gStaticData_086313AD:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006313AD, 0x00000003

.global gStaticData_086313B0
gStaticData_086313B0:
	@ LZ77 tile graphics (4bpp) (640 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/55_6313b0_tiles.4bpp.lz", 0, 0x140

.global gStaticData_086314F0
gStaticData_086314F0:
	@ LZ77 tile graphics (4bpp) (128 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/56_6314f0_tiles.4bpp.lz", 0, 0x67

gStaticData_08631557:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00631557, 0x00000001

.global gStaticData_08631558
gStaticData_08631558:
	@ LZ77 tile graphics (4bpp) (128 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/57_631558_tiles.4bpp.lz", 0, 0x63

gStaticData_086315BB:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006315BB, 0x00000001

.global gStaticData_086315BC
gStaticData_086315BC:
	@ LZ77 compressed data (1200 bytes decompressed) - the tilemap for the
	@ legal/credits text screen, gStaticData_0862D0CC (30x20 tiles, 16-bit
	@ entries).
	.incbin "build/crashbandicootxs/graphics/intro/58_6315bc.bin.lz", 0, 0x27E

gStaticData_0863183A:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0063183A, 0x00000002

.global gStaticData_0863183C
gStaticData_0863183C:
	@ LZ77 palette (256 colors) (512 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/59_63183c.gbapal.lz", 0, 0x162

gStaticData_0863199E:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0063199E, 0x00000002

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
	.incbin "build/crashbandicootxs/graphics/intro/68_631acc_tiles.4bpp.lz", 0, 0x9E7

gStaticData_086324B3:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006324B3, 0x00000001

.global gStaticData_086324B4
gStaticData_086324B4:
	@ LZ77 tile graphics (4bpp) (2048 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/69_6324b4_tiles.4bpp.lz", 0, 0x36A

gStaticData_0863281E:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0063281E, 0x00000002

.global gStaticData_08632820
gStaticData_08632820:
	@ LZ77 tile graphics (4bpp) (2048 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/70_632820_tiles.4bpp.lz", 0, 0x3A2

gStaticData_08632BC2:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00632BC2, 0x00000002

.global gStaticData_08632BC4
gStaticData_08632BC4:
	@ LZ77 tile graphics (4bpp) (6144 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/71_632bc4_tiles.4bpp.lz", 0, 0x8FD

gStaticData_086334C1:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006334C1, 0x00000003

.global gStaticData_086334C4
gStaticData_086334C4:
	@ LZ77 tile graphics (4bpp) (7680 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/72_6334c4_tiles.4bpp.lz", 0, 0xDAC

.global gStaticData_08634270
gStaticData_08634270:
	@ LZ77 tile graphics (4bpp) (25600 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/73_634270_tiles.4bpp.lz", 0, 0x2C81

gStaticData_08636EF1:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00636EF1, 0x00000003

.global gStaticData_08636EF4
gStaticData_08636EF4:
	@ LZ77 tile graphics (4bpp) (4608 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/74_636ef4_tiles.4bpp.lz", 0, 0x70F

gStaticData_08637603:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00637603, 0x00000001

.global gStaticData_08637604
gStaticData_08637604:
	@ LZ77 tile graphics (4bpp) (1024 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/75_637604_tiles.4bpp.lz", 0, 0x1B9

gStaticData_086377BD:
	@ trailing padding
	.incbin "baserom.gba", 0x006377BD, 0x00000003

.global gStaticData_086377C0
gStaticData_086377C0:
	@ LZ77 tile graphics (4bpp) (2048 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/00_6377c0_tiles.4bpp.lz", 0, 0x2AE

.global gStaticData_08637A6E
gStaticData_08637A6E:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00637A6E, 0x00000002

.global gStaticData_08637A70
gStaticData_08637A70:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gStaticData_0863CF98
	.incbin "build/crashbandicootxs/graphics/tileset1/01_637a70_8bpp_tiles.8bpp.lz", 0, 0x856

.global gStaticData_086382C6
gStaticData_086382C6:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006382C6, 0x00000002

.global gStaticData_086382C8
gStaticData_086382C8:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gStaticData_0863D01C
	.incbin "build/crashbandicootxs/graphics/tileset1/02_6382c8_8bpp_tiles.8bpp.lz", 0, 0xAA0

.global gStaticData_08638D68
gStaticData_08638D68:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gStaticData_0863D0A0
	.incbin "build/crashbandicootxs/graphics/tileset1/03_638d68_8bpp_tiles.8bpp.lz", 0, 0x6F1

.global gStaticData_08639459
gStaticData_08639459:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00639459, 0x00000003

.global gStaticData_0863945C
gStaticData_0863945C:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gStaticData_0863D124
	.incbin "build/crashbandicootxs/graphics/tileset1/04_63945c_8bpp_tiles.8bpp.lz", 0, 0x8BC

.global gStaticData_08639D18
gStaticData_08639D18:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gStaticData_0863D1A8
	.incbin "build/crashbandicootxs/graphics/tileset1/05_639d18_8bpp_tiles.8bpp.lz", 0, 0x8F2

.global gStaticData_0863A60A
gStaticData_0863A60A:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0063A60A, 0x00000002

.global gStaticData_0863A60C
gStaticData_0863A60C:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gStaticData_0863D22C
	.incbin "build/crashbandicootxs/graphics/tileset1/06_63a60c_8bpp_tiles.8bpp.lz", 0, 0x781

.global gStaticData_0863AD8D
gStaticData_0863AD8D:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0063AD8D, 0x00000003

.global gStaticData_0863AD90
gStaticData_0863AD90:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gStaticData_0863D2B0
	.incbin "build/crashbandicootxs/graphics/tileset1/07_63ad90_8bpp_tiles.8bpp.lz", 0, 0x8D6

.global gStaticData_0863B666
gStaticData_0863B666:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0063B666, 0x00000002

.global gStaticData_0863B668
gStaticData_0863B668:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gStaticData_0863D334
	.incbin "build/crashbandicootxs/graphics/tileset1/08_63b668_8bpp_tiles.8bpp.lz", 0, 0x76A

.global gStaticData_0863BDD2
gStaticData_0863BDD2:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0063BDD2, 0x00000002

.global gStaticData_0863BDD4
gStaticData_0863BDD4:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gStaticData_0863D3B8
	.incbin "build/crashbandicootxs/graphics/tileset1/09_63bdd4_8bpp_tiles.8bpp.lz", 0, 0x810

.global gStaticData_0863C5E4
gStaticData_0863C5E4:
	@ LZ77 tile graphics (8bpp) (4096 bytes decompressed) - a circular level-select
	@ icon vignette, using the palette at gStaticData_0863D43C
	.incbin "build/crashbandicootxs/graphics/tileset1/10_63c5e4_8bpp_tiles.8bpp.lz", 0, 0x9B1

.global gStaticData_0863CF95
gStaticData_0863CF95:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0063CF95, 0x00000003

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
	.incbin "build/crashbandicootxs/graphics/tileset1/17_63d2b0.bin.lz", 0, 0x83

.global gStaticData_0863D333
gStaticData_0863D333:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0063D333, 0x00000001

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
	.incbin "build/crashbandicootxs/graphics/tileset1/20_63d43c.bin.lz", 0, 0x83

.global gStaticData_0863D4BF
gStaticData_0863D4BF:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0063D4BF, 0x00000001

.global gStaticData_0863D4C0
gStaticData_0863D4C0:
	@ LZ77 tile graphics (4bpp) (23520 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/21_63d4c0_tiles.4bpp.lz", 0, 0x3A35

.global gStaticData_08640EF5
gStaticData_08640EF5:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00640EF5, 0x00000003

.global gStaticData_08640EF8
gStaticData_08640EF8:
	@ LZ77 tile graphics (4bpp) (23392 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/22_640ef8_tiles.4bpp.lz", 0, 0x3B8E

.global gStaticData_08644A86
gStaticData_08644A86:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00644A86, 0x00000002

.global gStaticData_08644A88
gStaticData_08644A88:
	@ LZ77 compressed data (24352 bytes decompressed) - not clearly identifiable as pixel graphics
	.incbin "build/crashbandicootxs/graphics/tileset1/23_644a88.bin.lz", 0, 0x498F

.global gStaticData_08649417
gStaticData_08649417:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00649417, 0x00000001

.global gStaticData_08649418
gStaticData_08649418:
	@ LZ77 compressed data (15968 bytes decompressed) - not clearly identifiable as pixel graphics
	.incbin "build/crashbandicootxs/graphics/tileset1/24_649418.bin.lz", 0, 0x2744

.global gStaticData_0864BB5C
gStaticData_0864BB5C:
	@ LZ77 compressed data (12576 bytes decompressed) - not clearly identifiable as pixel graphics
	.incbin "build/crashbandicootxs/graphics/tileset1/25_64bb5c.bin.lz", 0, 0x1C89

.global gStaticData_0864D7E5
gStaticData_0864D7E5:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0064D7E5, 0x00000003

.global gStaticData_0864D7E8
gStaticData_0864D7E8:
	@ LZ77 compressed data (14240 bytes decompressed) - not clearly identifiable as pixel graphics
	.incbin "build/crashbandicootxs/graphics/tileset1/26_64d7e8.bin.lz", 0, 0x2047

.global gStaticData_0864F82F
gStaticData_0864F82F:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0064F82F, 0x00000001

.global gStaticData_0864F830
gStaticData_0864F830:
	@ LZ77-packed level asset of room00_264730 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room00_264730/asset.bin.lz", 0, 0x24C7

.global gStaticData_08651CF7
gStaticData_08651CF7:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00651CF7, 0x00000001

.global gStaticData_08651CF8
gStaticData_08651CF8:
	@ LZ77-packed level asset of room01_263f4c (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room01_263f4c/asset.bin.lz", 0, 0x347D

.global gStaticData_08655175
gStaticData_08655175:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00655175, 0x00000003

.global gStaticData_08655178
gStaticData_08655178:
	@ LZ77-packed level asset of room02_2636e0 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room02_2636e0/asset.bin.lz", 0, 0x1BE8

.global gStaticData_08656D60
gStaticData_08656D60:
	@ LZ77-packed level asset of room04_262e64 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room04_262e64/asset.bin.lz", 0, 0x4D8B

.global gStaticData_0865BAEB
gStaticData_0865BAEB:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0065BAEB, 0x00000001

.global gStaticData_0865BAEC
gStaticData_0865BAEC:
	@ LZ77-packed level asset of room06_267428 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room06_267428/asset.bin.lz", 0, 0x256E

.global gStaticData_0865E05A
gStaticData_0865E05A:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0065E05A, 0x00000002

.global gStaticData_0865E05C
gStaticData_0865E05C:
	@ LZ77-packed level asset of room07_264e64 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room07_264e64/asset.bin.lz", 0, 0x3EEA

.global gStaticData_08661F46
gStaticData_08661F46:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00661F46, 0x00000002

.global gStaticData_08661F48
gStaticData_08661F48:
	@ LZ77-packed level asset of room09_262654 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room09_262654/asset.bin.lz", 0, 0x336E

.global gStaticData_086652B6
gStaticData_086652B6:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006652B6, 0x00000002

.global gStaticData_086652B8
gStaticData_086652B8:
	@ LZ77-packed level asset of room38_2bac1c (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room38_2bac1c/asset.bin.lz", 0, 0x67D

.global gStaticData_08665935
gStaticData_08665935:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00665935, 0x00000003

.global gStaticData_08665938
gStaticData_08665938:
	@ LZ77-packed level asset of room13_2bbde0 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room13_2bbde0/asset.bin.lz", 0, 0x4300

.global gStaticData_08669C38
gStaticData_08669C38:
	@ LZ77-packed level asset of room14_2bcf40 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room14_2bcf40/asset.bin.lz", 0, 0x4AC0

.global gStaticData_0866E6F8
gStaticData_0866E6F8:
	@ LZ77-packed level asset of room16_2beadc (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room16_2beadc/asset.bin.lz", 0, 0x1D66

.global gStaticData_0867045E
gStaticData_0867045E:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0067045E, 0x00000002

.global gStaticData_08670460
gStaticData_08670460:
	@ LZ77-packed level asset of room18_261c40 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room18_261c40/asset.bin.lz", 0, 0x7E3B

.global gStaticData_0867829B
gStaticData_0867829B:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0067829B, 0x00000001

.global gStaticData_0867829C
gStaticData_0867829C:
	@ LZ77-packed level asset of room20_25f11c (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room20_25f11c/asset.bin.lz", 0, 0x3508

.global gStaticData_0867B7A4
gStaticData_0867B7A4:
	@ LZ77-packed level asset of room21_25d81c (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room21_25d81c/asset.bin.lz", 0, 0x364F

.global gStaticData_0867EDF3
gStaticData_0867EDF3:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0067EDF3, 0x00000001

.global gStaticData_0867EDF4
gStaticData_0867EDF4:
	@ LZ77-packed level asset of room23_25cf20 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room23_25cf20/asset.bin.lz", 0, 0x4701

.global gStaticData_086834F5
gStaticData_086834F5:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006834F5, 0x00000003

.global gStaticData_086834F8
gStaticData_086834F8:
	@ LZ77-packed level asset of room24_25c640 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room24_25c640/asset.bin.lz", 0, 0x2F54

.global gStaticData_0868644C
gStaticData_0868644C:
	@ LZ77-packed level asset of room28_254ed0 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room28_254ed0/asset.bin.lz", 0, 0x3B26

.global gStaticData_08689F72
gStaticData_08689F72:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00689F72, 0x00000002

.global gStaticData_08689F74
gStaticData_08689F74:
	@ LZ77-packed level asset of room29_2544fc (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room29_2544fc/asset.bin.lz", 0, 0x339D

.global gStaticData_0868D311
gStaticData_0868D311:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0068D311, 0x00000003

.global gStaticData_0868D314
gStaticData_0868D314:
	@ LZ77-packed level asset of room30_26ac10 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room30_26ac10/asset.bin.lz", 0, 0x687A

.global gStaticData_08693B8E
gStaticData_08693B8E:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00693B8E, 0x00000002

.global gStaticData_08693B90
gStaticData_08693B90:
	@ LZ77-packed level asset of room31_2539b0 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room31_2539b0/asset.bin.lz", 0, 0x9C74

.global gStaticData_0869D804
gStaticData_0869D804:
	@ LZ77-packed level asset of room34_24c400 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room34_24c400/asset.bin.lz", 0, 0xAB45

.global gStaticData_086A8349
gStaticData_086A8349:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006A8349, 0x00000003

.global gStaticData_086A834C
gStaticData_086A834C:
	@ LZ77-packed level asset of room35_26c1bc (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room35_26c1bc/asset.bin.lz", 0, 0x6DDF

.global gStaticData_086AF12B
gStaticData_086AF12B:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006AF12B, 0x00000001

.global gStaticData_086AF12C
gStaticData_086AF12C:
	@ LZ77-packed level asset of room36_268cf0 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room36_268cf0/asset.bin.lz", 0, 0x6468

.global gStaticData_086B5594
gStaticData_086B5594:
	@ LZ77-packed level asset of room22_26e760 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room22_26e760/asset.bin.lz", 0, 0x571F

.global gStaticData_086BACB3
gStaticData_086BACB3:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006BACB3, 0x00000001

.global gStaticData_086BACB4
gStaticData_086BACB4:
	@ LZ77-packed level asset of room25_26d388 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room25_26d388/asset.bin.lz", 0, 0x65C8

.global gStaticData_086C127C
gStaticData_086C127C:
	@ raw level asset of room17_25e7dc (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room17_25e7dc/asset.bin"

.global gStaticData_086D9CAC
gStaticData_086D9CAC:
	@ LZ77-packed level asset of room37_2b9ed0 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room37_2b9ed0/asset.bin.lz", 0, 0x679F

.global gStaticData_086E044B
gStaticData_086E044B:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006E044B, 0x00000001

.global gStaticData_086E044C
gStaticData_086E044C:
	@ LZ77-packed level asset of room40_2bb094 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room40_2bb094/asset.bin.lz", 0, 0x988

.global gStaticData_086E0DD4
gStaticData_086E0DD4:
	@ LZ77-packed level asset of room03_270bcc (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room03_270bcc/asset.bin.lz", 0, 0x143E

.global gStaticData_086E2212
gStaticData_086E2212:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006E2212, 0x00000002

.global gStaticData_086E2214
gStaticData_086E2214:
	@ LZ77-packed level asset of room05_270154 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room05_270154/asset.bin.lz", 0, 0x132D

.global gStaticData_086E3541
gStaticData_086E3541:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006E3541, 0x00000003

.global gStaticData_086E3544
gStaticData_086E3544:
	@ LZ77-packed level asset of room11_26f5c0 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room11_26f5c0/asset.bin.lz", 0, 0x120C

.global gStaticData_086E4750
gStaticData_086E4750:
	@ LZ77-packed level asset of room08_265804 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room08_265804/asset.bin.lz", 0, 0x2908

.global gStaticData_086E7058
gStaticData_086E7058:
	@ LZ77-packed level asset of room10_266194 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room10_266194/asset.bin.lz", 0, 0x3071

.global gStaticData_086EA0C9
gStaticData_086EA0C9:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006EA0C9, 0x00000003

.global gStaticData_086EA0CC
gStaticData_086EA0CC:
	@ LZ77-packed level asset of room12_266b40 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room12_266b40/asset.bin.lz", 0, 0x2570

.global gStaticData_086EC63C
gStaticData_086EC63C:
	@ LZ77-packed level asset of room39_2ba810 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room39_2ba810/asset.bin.lz", 0, 0x696

.global gStaticData_086ECCD2
gStaticData_086ECCD2:
	@ padding
	.incbin "baserom.gba", 0x006ECCD2, 0x00000002

.global gStaticData_086ECCD4
gStaticData_086ECCD4:
	@ raw level asset of room15_2bdf98 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room15_2bdf98/asset.bin"

.global gStaticData_08708158
gStaticData_08708158:
	@ raw level asset of room19_260768 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room19_260768/asset.bin"

.global gStaticData_087376C0
gStaticData_087376C0:
	@ raw level asset of room26_25bcdc (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room26_25bcdc/asset.bin"

.global gStaticData_08752D44
gStaticData_08752D44:
	@ raw level asset of room27_25a390 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room27_25a390/asset.bin"

.global gStaticData_0878ADA8
gStaticData_0878ADA8:
	@ raw level asset of room32_24e104 (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room32_24e104/asset.bin"

.global gStaticData_087BC13C
gStaticData_087BC13C:
	@ raw level asset of room33_25233c (docs/levels.md)
	.incbin "build/crashbandicootxs/data/levels/room33_25233c/asset.bin"

@ gStaticData_087E3BEC..gStaticData_087E55C4: src/data/entity_vtables_7e3bec.c

@ 0x087E55E4..0x087E5FCC: the IWRAM image (asm/intr_main.s, src/iwram/),
@ linked to run at 0x03000000 - the `iwram` section in ldscript.txt.
@ 0x087E5FCC..0x08800000: 0xFF cartridge fill, the `rom_fill` section.

