.section .rodata

.global gStaticData_0803B8B0
gStaticData_0803B8B0:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0003B8B0, 0x00076870

.global gStaticData_080B2120
gStaticData_080B2120:
	@ LZ77 compressed data (unidentified) (213064 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/unknown/00_0b2120.bin.lz", 0, 0xEB16

.global gStaticData_080C0C36
gStaticData_080C0C36:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x000C0C36, 0x00080B16

.global gStaticData_0814174C
gStaticData_0814174C:
	@ LZ77 compressed data (unidentified) (207124 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/unknown/01_14174c.bin.lz", 0, 0x10376

.global gStaticData_08151AC2
gStaticData_08151AC2:
	@ padding/unidentified data
	.incbin "baserom.gba", 0x00151AC2, 0x00016012

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

.global gStaticData_0816AA20
gStaticData_0816AA20:
	.incbin "baserom.gba", 0x0016AA20, 0x0000004C

.global gStaticData_0816AA6C
gStaticData_0816AA6C:
	@ GAX2 sound-effect trigger table: 99 entries of {slot_id, pitch_offset,
	@ volume}, indexed by the sound effect IDs passed to sub_8001854. Reuses
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

.global gStaticData_0816B284
gStaticData_0816B284:
	.incbin "baserom.gba", 0x0016B284, 0x00000014

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

.global gStaticData_0816B92C
gStaticData_0816B92C:
	.incbin "baserom.gba", 0x0016B92C, 0x00000008

.global gStaticData_0816B934
gStaticData_0816B934:
	.incbin "baserom.gba", 0x0016B934, 0x00000008

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

.global gStaticData_0816BF20
gStaticData_0816BF20:
	.incbin "baserom.gba", 0x0016BF20, 0x00000150

.global gStaticData_0816C070
gStaticData_0816C070:
	.incbin "baserom.gba", 0x0016C070, 0x00000020

.global gStaticData_0816C090
gStaticData_0816C090:
	.incbin "baserom.gba", 0x0016C090, 0x000001C0

.global gStaticData_0816C250
gStaticData_0816C250:
	.incbin "baserom.gba", 0x0016C250, 0x00000040

.global gStaticData_0816C290
gStaticData_0816C290:
	.incbin "baserom.gba", 0x0016C290, 0x00000040

.global gStaticData_0816C2D0
gStaticData_0816C2D0:
	.incbin "baserom.gba", 0x0016C2D0, 0x00000008

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

.global gStaticData_0816C418
gStaticData_0816C418:
	.incbin "baserom.gba", 0x0016C418, 0x00000040

.global gStaticData_0816C458
gStaticData_0816C458:
	.incbin "baserom.gba", 0x0016C458, 0x00000008

.global gStaticData_0816C460
gStaticData_0816C460:
	.incbin "baserom.gba", 0x0016C460, 0x00000024

.global gStaticData_0816C484
gStaticData_0816C484:
	.incbin "baserom.gba", 0x0016C484, 0x00000014

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

.global gStaticData_0816C58C
gStaticData_0816C58C:
	.incbin "baserom.gba", 0x0016C58C, 0x00000014

.global gStaticData_0816C5A0
gStaticData_0816C5A0:
	.incbin "baserom.gba", 0x0016C5A0, 0x00000050

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

.global gStaticData_0816C6A4
gStaticData_0816C6A4:
	.incbin "baserom.gba", 0x0016C6A4, 0x00000170

.global gStaticData_0816C814
gStaticData_0816C814:
	.incbin "baserom.gba", 0x0016C814, 0x0000000A

.global gStaticData_0816C81E
gStaticData_0816C81E:
	.incbin "baserom.gba", 0x0016C81E, 0x00000012

.global gStaticData_0816C830
gStaticData_0816C830:
	.incbin "baserom.gba", 0x0016C830, 0x00000012

.global gStaticData_0816C842
gStaticData_0816C842:
	.incbin "baserom.gba", 0x0016C842, 0x00000020

.global gStaticData_0816C862
gStaticData_0816C862:
	.incbin "baserom.gba", 0x0016C862, 0x0000000A

.global gStaticData_0816C86C
gStaticData_0816C86C:
	.incbin "baserom.gba", 0x0016C86C, 0x00000514

.global gStaticData_0816CD80
gStaticData_0816CD80:
	.incbin "baserom.gba", 0x0016CD80, 0x00000474

.global gStaticData_0816D1F4
gStaticData_0816D1F4:
	.incbin "baserom.gba", 0x0016D1F4, 0x000053B4

.global gStaticData_081725A8
gStaticData_081725A8:
	.incbin "baserom.gba", 0x001725A8, 0x00000004

.global gStaticData_081725AC
gStaticData_081725AC:
	.incbin "baserom.gba", 0x001725AC, 0x00000008

.global gStaticData_081725B4
gStaticData_081725B4:
	.incbin "baserom.gba", 0x001725B4, 0x00000008

.global gStaticData_081725BC
gStaticData_081725BC:
	.incbin "baserom.gba", 0x001725BC, 0x00000008

.global gStaticData_081725C4
gStaticData_081725C4:
	.incbin "baserom.gba", 0x001725C4, 0x0000261C

.global gStaticData_08174BE0
gStaticData_08174BE0:
	.incbin "baserom.gba", 0x00174BE0, 0x0000008C

.global gStaticData_08174C6C
gStaticData_08174C6C:
	.incbin "baserom.gba", 0x00174C6C, 0x00000118

.global gStaticData_08174D84
gStaticData_08174D84:
	.incbin "baserom.gba", 0x00174D84, 0x00000050

.global gStaticData_08174DD4
gStaticData_08174DD4:
	.incbin "baserom.gba", 0x00174DD4, 0x000003B4

.global gStaticData_08175188
gStaticData_08175188:
	.incbin "baserom.gba", 0x00175188, 0x0000004C

.global gStaticData_081751D4
gStaticData_081751D4:
	.incbin "baserom.gba", 0x001751D4, 0x00000384

.global gStaticData_08175558
gStaticData_08175558:
	.incbin "baserom.gba", 0x00175558, 0x0000000C

.global gStaticData_08175564
gStaticData_08175564:
	.incbin "baserom.gba", 0x00175564, 0x00000020

.global gStaticData_08175584
gStaticData_08175584:
	.incbin "baserom.gba", 0x00175584, 0x00000140

.global gStaticData_081756C4
gStaticData_081756C4:
	.incbin "baserom.gba", 0x001756C4, 0x0000009C

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

.global gStaticData_0817A6B8
gStaticData_0817A6B8:
	.incbin "baserom.gba", 0x0017A6B8, 0x00000070

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

.global gStaticData_0817A840
gStaticData_0817A840:
	.incbin "baserom.gba", 0x0017A840, 0x00000010

.global gStaticData_0817A850
gStaticData_0817A850:
	.incbin "baserom.gba", 0x0017A850, 0x00000030

.global gStaticData_0817A880
gStaticData_0817A880:
	.incbin "baserom.gba", 0x0017A880, 0x000001EC

.global gStaticData_0817AA6C
gStaticData_0817AA6C:
	.incbin "baserom.gba", 0x0017AA6C, 0x00000020

.global gStaticData_0817AA8C
gStaticData_0817AA8C:
	.incbin "baserom.gba", 0x0017AA8C, 0x0000000C

.global gStaticData_0817AA98
gStaticData_0817AA98:
	.incbin "baserom.gba", 0x0017AA98, 0x00001728

.global gStaticData_0817C1C0
gStaticData_0817C1C0:
	.incbin "baserom.gba", 0x0017C1C0, 0x00000040

.global gStaticData_0817C200
gStaticData_0817C200:
	.incbin "baserom.gba", 0x0017C200, 0x00000060

.global gStaticData_0817C260
gStaticData_0817C260:
	.incbin "baserom.gba", 0x0017C260, 0x00000020

.global gStaticData_0817C280
gStaticData_0817C280:
	.incbin "baserom.gba", 0x0017C280, 0x00000038

.global gStaticData_0817C2B8
gStaticData_0817C2B8:
	.incbin "baserom.gba", 0x0017C2B8, 0x00000018

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

.global gStaticData_0817C3FC
gStaticData_0817C3FC:
	.incbin "baserom.gba", 0x0017C3FC, 0x00000018

.global gStaticData_0817C414
gStaticData_0817C414:
	.incbin "baserom.gba", 0x0017C414, 0x00000018

.global gStaticData_0817C42C
gStaticData_0817C42C:
	.incbin "baserom.gba", 0x0017C42C, 0x00000018

.global gStaticData_0817C444
gStaticData_0817C444:
	.incbin "baserom.gba", 0x0017C444, 0x0000000C

.global gStaticData_0817C450
gStaticData_0817C450:
	.incbin "baserom.gba", 0x0017C450, 0x00000010

.global gStaticData_0817C460
gStaticData_0817C460:
	.incbin "baserom.gba", 0x0017C460, 0x00000050

.global gStaticData_0817C4B0
gStaticData_0817C4B0:
	.incbin "baserom.gba", 0x0017C4B0, 0x0000000C

.global gStaticData_0817C4BC
gStaticData_0817C4BC:
	.incbin "baserom.gba", 0x0017C4BC, 0x0000000C

.global gStaticData_0817C4C8
gStaticData_0817C4C8:
	.incbin "baserom.gba", 0x0017C4C8, 0x00000018

.global gStaticData_0817C4E0
gStaticData_0817C4E0:
	.incbin "baserom.gba", 0x0017C4E0, 0x00000018

.global gStaticData_0817C4F8
gStaticData_0817C4F8:
	.incbin "baserom.gba", 0x0017C4F8, 0x00000018

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

.global gStaticData_0817C594
gStaticData_0817C594:
	.incbin "baserom.gba", 0x0017C594, 0x00000014

.global gStaticData_0817C5A8
gStaticData_0817C5A8:
	.incbin "baserom.gba", 0x0017C5A8, 0x00000014

.global gStaticData_0817C5BC
gStaticData_0817C5BC:
	.incbin "baserom.gba", 0x0017C5BC, 0x00000014

.global gStaticData_0817C5D0
gStaticData_0817C5D0:
	.incbin "baserom.gba", 0x0017C5D0, 0x0000096C

.global gStaticData_0817CF3C
gStaticData_0817CF3C:
	.incbin "baserom.gba", 0x0017CF3C, 0x00000004

.global gStaticData_0817CF40
gStaticData_0817CF40:
	.incbin "baserom.gba", 0x0017CF40, 0x00000064

.global gStaticData_0817CFA4
gStaticData_0817CFA4:
	.incbin "baserom.gba", 0x0017CFA4, 0x00000050

.global gStaticData_0817CFF4
gStaticData_0817CFF4:
	.incbin "baserom.gba", 0x0017CFF4, 0x00000040

.global gStaticData_0817D034
gStaticData_0817D034:
	.incbin "baserom.gba", 0x0017D034, 0x00000020

.global gStaticData_0817D054
gStaticData_0817D054:
	.incbin "baserom.gba", 0x0017D054, 0x00000020

.global gStaticData_0817D074
gStaticData_0817D074:
	.incbin "baserom.gba", 0x0017D074, 0x00000070

.global gStaticData_0817D0E4
gStaticData_0817D0E4:
	.incbin "baserom.gba", 0x0017D0E4, 0x000005B4

.global gStaticData_0817D698
gStaticData_0817D698:
	.incbin "baserom.gba", 0x0017D698, 0x00000028

.global gStaticData_0817D6C0
gStaticData_0817D6C0:
	.incbin "baserom.gba", 0x0017D6C0, 0x000000A8

.global gStaticData_0817D768
gStaticData_0817D768:
	.incbin "baserom.gba", 0x0017D768, 0x00000014

.global gStaticData_0817D77C
gStaticData_0817D77C:
	.incbin "baserom.gba", 0x0017D77C, 0x00000014

.global gStaticData_0817D790
gStaticData_0817D790:
	.incbin "baserom.gba", 0x0017D790, 0x00000014

.global gStaticData_0817D7A4
gStaticData_0817D7A4:
	.incbin "baserom.gba", 0x0017D7A4, 0x00000F70

.global gStaticData_0817E714
gStaticData_0817E714:
	.incbin "baserom.gba", 0x0017E714, 0x00000018

.global gStaticData_0817E72C
gStaticData_0817E72C:
	.incbin "baserom.gba", 0x0017E72C, 0x00000020

.global gStaticData_0817E74C
gStaticData_0817E74C:
	.incbin "baserom.gba", 0x0017E74C, 0x00000020

.global gStaticData_0817E76C
gStaticData_0817E76C:
	.incbin "baserom.gba", 0x0017E76C, 0x00000020

.global gStaticData_0817E78C
gStaticData_0817E78C:
	.incbin "baserom.gba", 0x0017E78C, 0x00326E74

.global gStaticData_084A5600
gStaticData_084A5600:
	.incbin "baserom.gba", 0x004A5600, 0x000B66B4

.global gStaticData_0855BCB4
gStaticData_0855BCB4:
	@ Shin'en GAX2 sound engine data: shared instrument/sample pool plus all
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
	.incbin "baserom.gba", 0x005A4C5C, 0x00000014

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
	@ header/index table for the graphics below (not yet understood)
	.incbin "baserom.gba", 0x005A9F10, 0x00000260

.global gStaticData_085AA170
gStaticData_085AA170:
	@ Mode 4 bitmap, 240x160 8bpp, LZ77 (38400 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/00_5aa170_bitmap.bin.lz", 0, 0x3A61

gStaticData_085ADBD1:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x005ADBD1, 0x00000203

.global gStaticData_085ADDD4
gStaticData_085ADDD4:
	@ Mode 4 bitmap, 240x160 8bpp, LZ77 (38400 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/01_5addd4_bitmap.bin.lz", 0, 0x570D

gStaticData_085B34E1:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x005B34E1, 0x00000203

.global gStaticData_085B36E4
gStaticData_085B36E4:
	@ Mode 4 bitmap, 240x160 8bpp, LZ77 (38400 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/02_5b36e4_bitmap.bin.lz", 0, 0x4BD8

gStaticData_085B82BC:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x005B82BC, 0x00000200

.global gStaticData_085B84BC
gStaticData_085B84BC:
	@ Mode 4 bitmap, 240x160 8bpp, LZ77 (38400 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/03_5b84bc_bitmap.bin.lz", 0, 0x422C

gStaticData_085BC6E8:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x005BC6E8, 0x00000200

.global gStaticData_085BC8E8
gStaticData_085BC8E8:
	@ Mode 4 bitmap, 240x160 8bpp, LZ77 (38400 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/04_5bc8e8_bitmap.bin.lz", 0, 0x504E

gStaticData_085C1936:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x005C1936, 0x00000202

.global gStaticData_085C1B38
gStaticData_085C1B38:
	@ Mode 4 bitmap, 240x160 8bpp, LZ77 (38400 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/05_5c1b38_bitmap.bin.lz", 0, 0x4C15

gStaticData_085C674D:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x005C674D, 0x00000203

.global gStaticData_085C6950
gStaticData_085C6950:
	@ Mode 4 bitmap, 240x160 8bpp, LZ77 (38400 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/06_5c6950_bitmap.bin.lz", 0, 0x480D

gStaticData_085CB15D:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x005CB15D, 0x00000203

.global gStaticData_085CB360
gStaticData_085CB360:
	@ Mode 4 bitmap, 240x160 8bpp, LZ77 (38400 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/07_5cb360_bitmap.bin.lz", 0, 0x4B6A

gStaticData_085CFECA:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x005CFECA, 0x00000202

.global gStaticData_085D00CC
gStaticData_085D00CC:
	@ Mode 4 bitmap, 240x160 8bpp, LZ77 (38400 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/08_5d00cc_bitmap.bin.lz", 0, 0x6465

gStaticData_085D6531:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x005D6531, 0x00000203

.global gStaticData_085D6734
gStaticData_085D6734:
	@ Mode 4 bitmap, 240x160 8bpp, LZ77 (38400 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/09_5d6734_bitmap.bin.lz", 0, 0x5031

gStaticData_085DB765:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x005DB765, 0x00000203

.global gStaticData_085DB968
gStaticData_085DB968:
	@ Mode 4 bitmap, 240x160 8bpp, LZ77 (38400 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/10_5db968_bitmap.bin.lz", 0, 0x4154

gStaticData_085DFABC:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x005DFABC, 0x00000200

.global gStaticData_085DFCBC
gStaticData_085DFCBC:
	@ Mode 4 bitmap, 240x160 8bpp, LZ77 (38400 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/11_5dfcbc_bitmap.bin.lz", 0, 0x584F

gStaticData_085E550B:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x005E550B, 0x00000201

.global gStaticData_085E570C
gStaticData_085E570C:
	@ Mode 4 bitmap, 240x160 8bpp, LZ77 (38400 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/12_5e570c_bitmap.bin.lz", 0, 0x5538

gStaticData_085EAC44:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x005EAC44, 0x00000200

.global gStaticData_085EAE44
gStaticData_085EAE44:
	@ Mode 4 bitmap, 240x160 8bpp, LZ77 (38400 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/13_5eae44_bitmap.bin.lz", 0, 0x5523

gStaticData_085F0367:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x005F0367, 0x00000201

.global gStaticData_085F0568
gStaticData_085F0568:
	@ Mode 4 bitmap, 240x160 8bpp, LZ77 (38400 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/14_5f0568_bitmap.bin.lz", 0, 0x4759

gStaticData_085F4CC1:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x005F4CC1, 0x00000203

.global gStaticData_085F4EC4
gStaticData_085F4EC4:
	@ Mode 4 bitmap, 240x160 8bpp, LZ77 (38400 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/15_5f4ec4_bitmap.bin.lz", 0, 0x530E

gStaticData_085FA1D2:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x005FA1D2, 0x00000202

.global gStaticData_085FA3D4
gStaticData_085FA3D4:
	@ Mode 4 bitmap, 240x160 8bpp, LZ77 (38400 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/16_5fa3d4_bitmap.bin.lz", 0, 0x53A5

gStaticData_085FF779:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x005FF779, 0x00000203

.global gStaticData_085FF97C
gStaticData_085FF97C:
	@ Mode 4 bitmap, 240x160 8bpp, LZ77 (38400 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/17_5ff97c_bitmap.bin.lz", 0, 0x4036

gStaticData_086039B2:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006039B2, 0x00000202

.global gStaticData_08603BB4
gStaticData_08603BB4:
	@ Mode 4 bitmap, 240x160 8bpp, LZ77 (38400 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/18_603bb4_bitmap.bin.lz", 0, 0x4E9C

gStaticData_08608A50:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00608A50, 0x00000200

.global gStaticData_08608C50
gStaticData_08608C50:
	@ Mode 4 bitmap, 240x160 8bpp, LZ77 (38400 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/19_608c50_bitmap.bin.lz", 0, 0x4927

gStaticData_0860D577:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0060D577, 0x00000201

.global gStaticData_0860D778
gStaticData_0860D778:
	@ Mode 4 bitmap, 240x160 8bpp, LZ77 (38400 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/20_60d778_bitmap.bin.lz", 0, 0x4461

gStaticData_08611BD9:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00611BD9, 0x00000203

.global gStaticData_08611DDC
gStaticData_08611DDC:
	@ Mode 4 bitmap, 240x160 8bpp, LZ77 (38400 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/21_611ddc_bitmap.bin.lz", 0, 0x4584

gStaticData_08616360:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00616360, 0x00000200

.global gStaticData_08616560
gStaticData_08616560:
	@ Mode 4 bitmap, 240x160 8bpp, LZ77 (38400 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/22_616560_bitmap.bin.lz", 0, 0x39F1

gStaticData_08619F51:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00619F51, 0x00000203

.global gStaticData_0861A154
gStaticData_0861A154:
	@ Mode 4 bitmap, 240x160 8bpp, LZ77 (38400 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/intro/23_61a154_bitmap.bin.lz", 0, 0x1988

.global gStaticData_0861BADC
gStaticData_0861BADC:
	@ LZ77 palette (16 colors) (32 bytes decompressed) - the palette for
	@ gStaticData_0861C30C (sky/clouds background), loaded together as a
	@ {w,h,palette_ptr,tile_ptr,tilemap_ptr} package at gStaticData_0816C484
	@ via sub_801E578. Originally misclassified as a 1-tile 4bpp graphic (32
	@ bytes coincidentally matches one 4bpp tile).
	.incbin "build/crashbandicootxs/graphics/intro/24_61badc.gbapal.lz", 0, 0x28

.global gStaticData_0861BB04
gStaticData_0861BB04:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for gStaticData_0861E5F8 (Crash's face in a badge, warp-room
	@ background), loaded as a package at gStaticData_0816B284 via
	@ sub_801E578. Kept as raw binary: some entries have a stray set bit 15
	@ that a standard .pal round-trip through gbagfx can't reproduce
	@ (RGB555 only uses bits 0-14), which broke byte-exact rebuilding when
	@ tried as .pal
	.incbin "build/crashbandicootxs/graphics/intro/25_61bb04.bin.lz", 0, 0x244

.global gStaticData_0861BD48
gStaticData_0861BD48:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for gStaticData_0862556C (a level-select platform icon: a
	@ blue gem pool, palm trees, small ruins), loaded as a package at
	@ gStaticData_0816C58C via sub_801E578. Kept as raw binary: some
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
	@ at gStaticData_0817C594 via sub_801E578. Originally misclassified as
	@ a 1-tile 4bpp graphic (32 bytes coincidentally matches one 4bpp tile).
	.incbin "build/crashbandicootxs/graphics/intro/27_61bf30.gbapal.lz", 0, 0x28

.global gStaticData_0861BF58
gStaticData_0861BF58:
	@ LZ77 palette (16 colors) (32 bytes decompressed) - the palette for
	@ gStaticData_0862A958 (a fire/aura glow effect: green transparent
	@ background, orange/red/magenta outline), loaded as a package at
	@ gStaticData_0817C5A8 via sub_801E578. Originally misclassified as a
	@ 1-tile 4bpp graphic (32 bytes coincidentally matches one 4bpp tile).
	.incbin "build/crashbandicootxs/graphics/intro/28_61bf58.gbapal.lz", 0, 0x28

.global gStaticData_0861BF80
gStaticData_0861BF80:
	@ LZ77 compressed data (512 bytes decompressed) - the 256-color RGB555
	@ palette for gStaticData_0862B34C (Uka Uka's mask), loaded as a
	@ package at gStaticData_0817C5BC via sub_801E578. Kept as raw binary:
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
	@ package struct at gStaticData_0817D0E4, loaded onto BG2 by sub_80355E0.
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
	@ LZ77 compressed data (unidentified) (18884 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/27_64f830.bin.lz", 0, 0x24C7

.global gStaticData_08651CF7
gStaticData_08651CF7:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00651CF7, 0x00000001

.global gStaticData_08651CF8
gStaticData_08651CF8:
	@ LZ77 compressed data (unidentified) (29084 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/28_651cf8.bin.lz", 0, 0x347D

.global gStaticData_08655175
gStaticData_08655175:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00655175, 0x00000003

.global gStaticData_08655178
gStaticData_08655178:
	@ LZ77 compressed data (15200 bytes decompressed) - not clearly identifiable as pixel graphics
	.incbin "build/crashbandicootxs/graphics/tileset1/29_655178.bin.lz", 0, 0x1BE8

.global gStaticData_08656D60
gStaticData_08656D60:
	@ LZ77 compressed data (unidentified) (33892 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/30_656d60.bin.lz", 0, 0x4D8B

.global gStaticData_0865BAEB
gStaticData_0865BAEB:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0065BAEB, 0x00000001

.global gStaticData_0865BAEC
gStaticData_0865BAEC:
	@ LZ77 compressed data (unidentified) (21660 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/31_65baec.bin.lz", 0, 0x256E

.global gStaticData_0865E05A
gStaticData_0865E05A:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0065E05A, 0x00000002

.global gStaticData_0865E05C
gStaticData_0865E05C:
	@ LZ77 compressed data (unidentified) (25660 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/32_65e05c.bin.lz", 0, 0x3EEA

.global gStaticData_08661F46
gStaticData_08661F46:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00661F46, 0x00000002

.global gStaticData_08661F48
gStaticData_08661F48:
	@ LZ77 compressed data (unidentified) (30164 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/33_661f48.bin.lz", 0, 0x336E

.global gStaticData_086652B6
gStaticData_086652B6:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006652B6, 0x00000002

.global gStaticData_086652B8
gStaticData_086652B8:
	@ LZ77 compressed data (unidentified) (2576 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/34_6652b8.bin.lz", 0, 0x67D

.global gStaticData_08665935
gStaticData_08665935:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00665935, 0x00000003

.global gStaticData_08665938
gStaticData_08665938:
	@ LZ77 compressed data (unidentified) (34552 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/35_665938.bin.lz", 0, 0x4300

.global gStaticData_08669C38
gStaticData_08669C38:
	@ LZ77 compressed data (unidentified) (62512 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/36_669c38.bin.lz", 0, 0x4AC0

.global gStaticData_0866E6F8
gStaticData_0866E6F8:
	@ LZ77 compressed data (unidentified) (29720 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/37_66e6f8.bin.lz", 0, 0x1D66

.global gStaticData_0867045E
gStaticData_0867045E:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0067045E, 0x00000002

.global gStaticData_08670460
gStaticData_08670460:
	@ LZ77 compressed data (unidentified) (80172 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/38_670460.bin.lz", 0, 0x7E3B

.global gStaticData_0867829B
gStaticData_0867829B:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0067829B, 0x00000001

.global gStaticData_0867829C
gStaticData_0867829C:
	@ LZ77 compressed data (unidentified) (26060 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/39_67829c.bin.lz", 0, 0x3508

.global gStaticData_0867B7A4
gStaticData_0867B7A4:
	@ LZ77 compressed data (unidentified) (28840 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/40_67b7a4.bin.lz", 0, 0x364F

.global gStaticData_0867EDF3
gStaticData_0867EDF3:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0067EDF3, 0x00000001

.global gStaticData_0867EDF4
gStaticData_0867EDF4:
	@ LZ77 compressed data (unidentified) (36248 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/41_67edf4.bin.lz", 0, 0x4701

.global gStaticData_086834F5
gStaticData_086834F5:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006834F5, 0x00000003

.global gStaticData_086834F8
gStaticData_086834F8:
	@ LZ77 compressed data (unidentified) (24468 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/42_6834f8.bin.lz", 0, 0x2F54

.global gStaticData_0868644C
gStaticData_0868644C:
	@ LZ77 compressed data (unidentified) (32316 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/43_68644c.bin.lz", 0, 0x3B26

.global gStaticData_08689F72
gStaticData_08689F72:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00689F72, 0x00000002

.global gStaticData_08689F74
gStaticData_08689F74:
	@ LZ77 compressed data (unidentified) (28812 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/44_689f74.bin.lz", 0, 0x339D

.global gStaticData_0868D311
gStaticData_0868D311:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x0068D311, 0x00000003

.global gStaticData_0868D314
gStaticData_0868D314:
	@ LZ77 compressed data (unidentified) (77604 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/45_68d314.bin.lz", 0, 0x687A

.global gStaticData_08693B8E
gStaticData_08693B8E:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x00693B8E, 0x00000002

.global gStaticData_08693B90
gStaticData_08693B90:
	@ LZ77 compressed data (unidentified) (93284 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/46_693b90.bin.lz", 0, 0x9C74

.global gStaticData_0869D804
gStaticData_0869D804:
	@ LZ77 compressed data (unidentified) (92328 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/47_69d804.bin.lz", 0, 0xAB45

.global gStaticData_086A8349
gStaticData_086A8349:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006A8349, 0x00000003

.global gStaticData_086A834C
gStaticData_086A834C:
	@ LZ77 compressed data (unidentified) (69436 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/48_6a834c.bin.lz", 0, 0x6DDF

.global gStaticData_086AF12B
gStaticData_086AF12B:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006AF12B, 0x00000001

.global gStaticData_086AF12C
gStaticData_086AF12C:
	@ LZ77 compressed data (73120 bytes decompressed) - not clearly identifiable as pixel graphics
	.incbin "build/crashbandicootxs/graphics/tileset1/49_6af12c.bin.lz", 0, 0x6468

.global gStaticData_086B5594
gStaticData_086B5594:
	@ LZ77 compressed data (unidentified) (56700 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/50_6b5594.bin.lz", 0, 0x571F

.global gStaticData_086BACB3
gStaticData_086BACB3:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006BACB3, 0x00000001

.global gStaticData_086BACB4
gStaticData_086BACB4:
	@ LZ77 compressed data (unidentified) (70196 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/51_6bacb4.bin.lz", 0, 0x65C8

.global gStaticData_086C127C
gStaticData_086C127C:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006C127C, 0x00018A30

.global gStaticData_086D9CAC
gStaticData_086D9CAC:
	@ LZ77 compressed data (unidentified) (84396 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/52_6d9cac.bin.lz", 0, 0x679F

.global gStaticData_086E044B
gStaticData_086E044B:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006E044B, 0x00000001

.global gStaticData_086E044C
gStaticData_086E044C:
	@ LZ77 compressed data (unidentified) (3900 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/53_6e044c.bin.lz", 0, 0x988

.global gStaticData_086E0DD4
gStaticData_086E0DD4:
	@ LZ77 compressed data (15904 bytes decompressed) - not clearly identifiable as pixel graphics
	.incbin "build/crashbandicootxs/graphics/tileset1/54_6e0dd4.bin.lz", 0, 0x143E

.global gStaticData_086E2212
gStaticData_086E2212:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006E2212, 0x00000002

.global gStaticData_086E2214
gStaticData_086E2214:
	@ LZ77 compressed data (13952 bytes decompressed) - not clearly identifiable as pixel graphics
	.incbin "build/crashbandicootxs/graphics/tileset1/55_6e2214.bin.lz", 0, 0x132D

.global gStaticData_086E3541
gStaticData_086E3541:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006E3541, 0x00000003

.global gStaticData_086E3544
gStaticData_086E3544:
	@ LZ77 compressed data (14048 bytes decompressed) - not clearly identifiable as pixel graphics
	.incbin "build/crashbandicootxs/graphics/tileset1/56_6e3544.bin.lz", 0, 0x120C

.global gStaticData_086E4750
gStaticData_086E4750:
	@ LZ77 compressed data (33280 bytes decompressed) - not clearly identifiable as pixel graphics
	.incbin "build/crashbandicootxs/graphics/tileset1/57_6e4750.bin.lz", 0, 0x2908

.global gStaticData_086E7058
gStaticData_086E7058:
	@ LZ77 compressed data (unidentified) (36176 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/58_6e7058.bin.lz", 0, 0x3071

.global gStaticData_086EA0C9
gStaticData_086EA0C9:
	@ padding/unidentified data between assets
	.incbin "baserom.gba", 0x006EA0C9, 0x00000003

.global gStaticData_086EA0CC
gStaticData_086EA0CC:
	@ LZ77 compressed data (unidentified) (31504 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/59_6ea0cc.bin.lz", 0, 0x2570

.global gStaticData_086EC63C
gStaticData_086EC63C:
	@ LZ77 compressed data (unidentified) (4184 bytes decompressed)
	.incbin "build/crashbandicootxs/graphics/tileset1/60_6ec63c.bin.lz", 0, 0x696

.global gStaticData_086ECCD2
gStaticData_086ECCD2:
	@ padding/unidentified data
	.incbin "baserom.gba", 0x006ECCD2, 0x000F6F1A

.global gStaticData_087E3BEC
gStaticData_087E3BEC:
	.incbin "baserom.gba", 0x007E3BEC, 0x00000058

.global gStaticData_087E3C44
gStaticData_087E3C44:
	.incbin "baserom.gba", 0x007E3C44, 0x00000068

.global gStaticData_087E3CAC
gStaticData_087E3CAC:
	.incbin "baserom.gba", 0x007E3CAC, 0x00000068

.global gStaticData_087E3D14
gStaticData_087E3D14:
	.incbin "baserom.gba", 0x007E3D14, 0x00000078

.global gStaticData_087E3D8C
gStaticData_087E3D8C:
	.incbin "baserom.gba", 0x007E3D8C, 0x00000078

.global gStaticData_087E3E04
gStaticData_087E3E04:
	.incbin "baserom.gba", 0x007E3E04, 0x00000078

.global gStaticData_087E3E7C
gStaticData_087E3E7C:
	.incbin "baserom.gba", 0x007E3E7C, 0x00000068

.global gStaticData_087E3EE4
gStaticData_087E3EE4:
	.incbin "baserom.gba", 0x007E3EE4, 0x00000068

.global gStaticData_087E3F4C
gStaticData_087E3F4C:
	.incbin "baserom.gba", 0x007E3F4C, 0x00000058

.global gStaticData_087E3FA4
gStaticData_087E3FA4:
	.incbin "baserom.gba", 0x007E3FA4, 0x00000068

.global gStaticData_087E400C
gStaticData_087E400C:
	.incbin "baserom.gba", 0x007E400C, 0x00000068

.global gStaticData_087E4074
gStaticData_087E4074:
	.incbin "baserom.gba", 0x007E4074, 0x00000068

.global gStaticData_087E40DC
gStaticData_087E40DC:
	.incbin "baserom.gba", 0x007E40DC, 0x00000070

.global gStaticData_087E414C
gStaticData_087E414C:
	.incbin "baserom.gba", 0x007E414C, 0x00000070

.global gStaticData_087E41BC
gStaticData_087E41BC:
	.incbin "baserom.gba", 0x007E41BC, 0x00000068

.global gStaticData_087E4224
gStaticData_087E4224:
	.incbin "baserom.gba", 0x007E4224, 0x00000068

.global gStaticData_087E428C
gStaticData_087E428C:
	.incbin "baserom.gba", 0x007E428C, 0x00000068

.global gStaticData_087E42F4
gStaticData_087E42F4:
	.incbin "baserom.gba", 0x007E42F4, 0x00000068

.global gStaticData_087E435C
gStaticData_087E435C:
	.incbin "baserom.gba", 0x007E435C, 0x00000068

.global gStaticData_087E43C4
gStaticData_087E43C4:
	.incbin "baserom.gba", 0x007E43C4, 0x00000068

.global gStaticData_087E442C
gStaticData_087E442C:
	.incbin "baserom.gba", 0x007E442C, 0x00000068

.global gStaticData_087E4494
gStaticData_087E4494:
	.incbin "baserom.gba", 0x007E4494, 0x00000068

.global gStaticData_087E44FC
gStaticData_087E44FC:
	.incbin "baserom.gba", 0x007E44FC, 0x00000068

.global gStaticData_087E4564
gStaticData_087E4564:
	.incbin "baserom.gba", 0x007E4564, 0x00000068

.global gStaticData_087E45CC
gStaticData_087E45CC:
	.incbin "baserom.gba", 0x007E45CC, 0x00000068

.global gStaticData_087E4634
gStaticData_087E4634:
	.incbin "baserom.gba", 0x007E4634, 0x00000068

.global gStaticData_087E469C
gStaticData_087E469C:
	.incbin "baserom.gba", 0x007E469C, 0x00000068

.global gStaticData_087E4704
gStaticData_087E4704:
	.incbin "baserom.gba", 0x007E4704, 0x00000068

.global gStaticData_087E476C
gStaticData_087E476C:
	.incbin "baserom.gba", 0x007E476C, 0x00000068

.global gStaticData_087E47D4
gStaticData_087E47D4:
	.incbin "baserom.gba", 0x007E47D4, 0x00000068

.global gStaticData_087E483C
gStaticData_087E483C:
	.incbin "baserom.gba", 0x007E483C, 0x00000068

.global gStaticData_087E48A4
gStaticData_087E48A4:
	.incbin "baserom.gba", 0x007E48A4, 0x00000068

.global gStaticData_087E490C
gStaticData_087E490C:
	.incbin "baserom.gba", 0x007E490C, 0x00000068

.global gStaticData_087E4974
gStaticData_087E4974:
	.incbin "baserom.gba", 0x007E4974, 0x00000068

.global gStaticData_087E49DC
gStaticData_087E49DC:
	.incbin "baserom.gba", 0x007E49DC, 0x00000078

.global gStaticData_087E4A54
gStaticData_087E4A54:
	.incbin "baserom.gba", 0x007E4A54, 0x00000068

.global gStaticData_087E4ABC
gStaticData_087E4ABC:
	.incbin "baserom.gba", 0x007E4ABC, 0x00000078

.global gStaticData_087E4B34
gStaticData_087E4B34:
	.incbin "baserom.gba", 0x007E4B34, 0x00000078

.global gStaticData_087E4BAC
gStaticData_087E4BAC:
	.incbin "baserom.gba", 0x007E4BAC, 0x00000030

.global gStaticData_087E4BDC
gStaticData_087E4BDC:
	.incbin "baserom.gba", 0x007E4BDC, 0x00000010

.global gStaticData_087E4BEC
gStaticData_087E4BEC:
	.incbin "baserom.gba", 0x007E4BEC, 0x00000028

.global gStaticData_087E4C14
gStaticData_087E4C14:
	.incbin "baserom.gba", 0x007E4C14, 0x00000050

.global gStaticData_087E4C64
gStaticData_087E4C64:
	.incbin "baserom.gba", 0x007E4C64, 0x00000050

.global gStaticData_087E4CB4
gStaticData_087E4CB4:
	.incbin "baserom.gba", 0x007E4CB4, 0x00000068

.global gStaticData_087E4D1C
gStaticData_087E4D1C:
	.incbin "baserom.gba", 0x007E4D1C, 0x00000048

.global gStaticData_087E4D64
gStaticData_087E4D64:
	.incbin "baserom.gba", 0x007E4D64, 0x00000048

.global gStaticData_087E4DAC
gStaticData_087E4DAC:
	.incbin "baserom.gba", 0x007E4DAC, 0x00000048

.global gStaticData_087E4DF4
gStaticData_087E4DF4:
	.incbin "baserom.gba", 0x007E4DF4, 0x00000020

.global gStaticData_087E4E14
gStaticData_087E4E14:
	.incbin "baserom.gba", 0x007E4E14, 0x00000020

.global gStaticData_087E4E34
gStaticData_087E4E34:
	.incbin "baserom.gba", 0x007E4E34, 0x00000020

.global gStaticData_087E4E54
gStaticData_087E4E54:
	.incbin "baserom.gba", 0x007E4E54, 0x00000020

.global gStaticData_087E4E74
gStaticData_087E4E74:
	.incbin "baserom.gba", 0x007E4E74, 0x00000020

.global gStaticData_087E4E94
gStaticData_087E4E94:
	.incbin "baserom.gba", 0x007E4E94, 0x00000020

.global gStaticData_087E4EB4
gStaticData_087E4EB4:
	.incbin "baserom.gba", 0x007E4EB4, 0x00000020

.global gStaticData_087E4ED4
gStaticData_087E4ED4:
	.incbin "baserom.gba", 0x007E4ED4, 0x00000020

.global gStaticData_087E4EF4
gStaticData_087E4EF4:
	.incbin "baserom.gba", 0x007E4EF4, 0x00000020

.global gStaticData_087E4F14
gStaticData_087E4F14:
	.incbin "baserom.gba", 0x007E4F14, 0x00000020

.global gStaticData_087E4F34
gStaticData_087E4F34:
	.incbin "baserom.gba", 0x007E4F34, 0x00000020

.global gStaticData_087E4F54
gStaticData_087E4F54:
	.incbin "baserom.gba", 0x007E4F54, 0x00000020

.global gStaticData_087E4F74
gStaticData_087E4F74:
	.incbin "baserom.gba", 0x007E4F74, 0x00000020

.global gStaticData_087E4F94
gStaticData_087E4F94:
	.incbin "baserom.gba", 0x007E4F94, 0x00000020

.global gStaticData_087E4FB4
gStaticData_087E4FB4:
	.incbin "baserom.gba", 0x007E4FB4, 0x00000020

.global gStaticData_087E4FD4
gStaticData_087E4FD4:
	.incbin "baserom.gba", 0x007E4FD4, 0x00000020

.global gStaticData_087E4FF4
gStaticData_087E4FF4:
	.incbin "baserom.gba", 0x007E4FF4, 0x00000020

.global gStaticData_087E5014
gStaticData_087E5014:
	.incbin "baserom.gba", 0x007E5014, 0x00000020

.global gStaticData_087E5034
gStaticData_087E5034:
	.incbin "baserom.gba", 0x007E5034, 0x00000020

.global gStaticData_087E5054
gStaticData_087E5054:
	.incbin "baserom.gba", 0x007E5054, 0x00000020

.global gStaticData_087E5074
gStaticData_087E5074:
	.incbin "baserom.gba", 0x007E5074, 0x00000020

.global gStaticData_087E5094
gStaticData_087E5094:
	.incbin "baserom.gba", 0x007E5094, 0x00000020

.global gStaticData_087E50B4
gStaticData_087E50B4:
	.incbin "baserom.gba", 0x007E50B4, 0x00000020

.global gStaticData_087E50D4
gStaticData_087E50D4:
	.incbin "baserom.gba", 0x007E50D4, 0x00000038

.global gStaticData_087E510C
gStaticData_087E510C:
	.incbin "baserom.gba", 0x007E510C, 0x00000038

.global gStaticData_087E5144
gStaticData_087E5144:
	.incbin "baserom.gba", 0x007E5144, 0x00000038

.global gStaticData_087E517C
gStaticData_087E517C:
	.incbin "baserom.gba", 0x007E517C, 0x00000038

.global gStaticData_087E51B4
gStaticData_087E51B4:
	.incbin "baserom.gba", 0x007E51B4, 0x00000038

.global gStaticData_087E51EC
gStaticData_087E51EC:
	.incbin "baserom.gba", 0x007E51EC, 0x00000038

.global gStaticData_087E5224
gStaticData_087E5224:
	.incbin "baserom.gba", 0x007E5224, 0x00000038

.global gStaticData_087E525C
gStaticData_087E525C:
	.incbin "baserom.gba", 0x007E525C, 0x00000038

.global gStaticData_087E5294
gStaticData_087E5294:
	.incbin "baserom.gba", 0x007E5294, 0x00000038

.global gStaticData_087E52CC
gStaticData_087E52CC:
	.incbin "baserom.gba", 0x007E52CC, 0x00000040

.global gStaticData_087E530C
gStaticData_087E530C:
	.incbin "baserom.gba", 0x007E530C, 0x00000040

.global gStaticData_087E534C
gStaticData_087E534C:
	.incbin "baserom.gba", 0x007E534C, 0x00000040

.global gStaticData_087E538C
gStaticData_087E538C:
	.incbin "baserom.gba", 0x007E538C, 0x00000040

.global gStaticData_087E53CC
gStaticData_087E53CC:
	.incbin "baserom.gba", 0x007E53CC, 0x00000038

.global gStaticData_087E5404
gStaticData_087E5404:
	.incbin "baserom.gba", 0x007E5404, 0x00000038

.global gStaticData_087E543C
gStaticData_087E543C:
	.incbin "baserom.gba", 0x007E543C, 0x00000038

.global gStaticData_087E5474
gStaticData_087E5474:
	.incbin "baserom.gba", 0x007E5474, 0x00000038

.global gStaticData_087E54AC
gStaticData_087E54AC:
	.incbin "baserom.gba", 0x007E54AC, 0x00000038

.global gStaticData_087E54E4
gStaticData_087E54E4:
	.incbin "baserom.gba", 0x007E54E4, 0x00000038

.global gStaticData_087E551C
gStaticData_087E551C:
	.incbin "baserom.gba", 0x007E551C, 0x00000038

.global gStaticData_087E5554
gStaticData_087E5554:
	.incbin "baserom.gba", 0x007E5554, 0x00000038

.global gStaticData_087E558C
gStaticData_087E558C:
	.incbin "baserom.gba", 0x007E558C, 0x00000038

.global gStaticData_087E55C4
gStaticData_087E55C4:
	.incbin "baserom.gba", 0x007E55C4, 0x00000020

.global gStaticData_087E55E4
gStaticData_087E55E4:
	.incbin "baserom.gba", 0x007E55E4, 0x0001AA1C

