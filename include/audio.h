#ifndef __AUDIO_H__
#define __AUDIO_H__

/* The audio front end's C-linkage data and functions. The audio context
 * itself is C++, class AudioContext (include/audio.hpp, with its field
 * list); C (src/data/song_table_16aa20.c) needs the song table and the
 * music block here. */

#include "core.h"
#include <gax.h>
#include "constants/sfx.h"
#include "constants/songs.h"

/* One record of the 99-entry sound-effect trigger table at ROM
 * `0x0816AA6C` (`sound/sfx_table.json`) - see docs/audio.md's "Sound
 * effects" section. Indexed by the id `PlaySfx`/`PlayAmbientSfx` take. */
struct SfxTableEntry {
    /* instrument index into the sound-effect data set
     * (gGaxSfxData, docs/audio.md) - 0 = unused slot */
    u32 slotId;
    /* passed through as GAX_fx_ex's priority arg (only
     * PlaySfx reads this; PlayAmbientSfx hardcodes 0) */
    u32 chanArg;
    /* multiplied by the caller's volume param and AudioContext.sfxVolume, then >>16 */
    u32 baseVolume;
};

extern struct SfxTableEntry gSfxTable[99];

/* The GAX2 music block (data/data.s, built by tools/gax_audio.py) and
 * the song table pointing into it (src/data/song_table_16aa20.c). */
extern const u8 gGaxMusicData[];
extern const void *const gSongTable[SONG_COUNT];

/* src/iwram/iwram_data.cpp */
/* VBlankHandler (src/system/irq.cpp) calls GAX_irq while this is set. */
extern u8 gGaxIrqEnabled;
/* PlaySfx's two-voice round robin. */
extern u32 gSfxVoiceToggle;

/* src/audio/audio.cpp */
/* The two functions with C linkage: the IRQ table points at the handler.
 * The methods' C names (cxx_symbols.txt) have no C caller left. */
extern void EnableMusicVCountIrq(void);
extern void MusicVCountIrqHandler(void);

#endif /* __AUDIO_H__ */
