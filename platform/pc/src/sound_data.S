/* The sound blocks of games/xs/data/data.s, in place of the GBA's data.s:
 * gSfxTable (AudioContext's sound-effect table), the music block
 * gGaxMusicData (whose start AudioContext hands to GAX2 as the song's
 * sfxTypes, and whose songs gSongTable points at) and the sound-effect
 * set gGaxSfxData. All are the built bytes (platform.mk passes their
 * paths), with zeros where the GBA's have their pointers (tools/
 * gax_audio.py's .s files carry those as relocations): the platform's
 * GAX2 plays the structures with pointers from gax_host_data.c
 * (tools/gax_host_data.py), and reads the rest (patterns, envelopes,
 * samples, ...) here in place. */

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

    .balign 4
    .globl gGaxSfxData
gGaxSfxData:
    .incbin GAX_SFX_BIN

    .section .note.GNU-stack,"",@progbits
