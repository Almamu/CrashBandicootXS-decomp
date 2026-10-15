/* The two sound blocks of data/data.s the game itself reads, in place of
 * the GBA's data.s: gSfxTable (AudioContext's sound-effect table) and
 * gGaxMusicData (whose start AudioContext hands to GAX2 as the song's
 * sfxTypes). Both are the built bytes (platform.mk passes their paths).
 * gGaxMusicData's pointers stay the GBA's addresses: reading the music
 * data is the job of the platform's GAX2 (phase 6 of #1093). */

    .data
    .balign 4
    .globl gSfxTable
gSfxTable:
    .incbin SFX_TABLE_BIN

    .section .rodata
    .balign 4
    .globl gGaxMusicData
gGaxMusicData:
    .incbin GAX_MUSIC_BIN

    .section .note.GNU-stack,"",@progbits
