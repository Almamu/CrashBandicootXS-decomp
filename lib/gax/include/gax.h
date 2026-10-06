#ifndef __GAX_H__
#define __GAX_H__

#include "gba/types.h"

/* GAX Sound Engine 2.01D (Shin'en Multimedia, Sep 28 2001), the game's
 * music and sound-effect driver: the public interface the game calls.
 * The engine is lib/gax (docs/audio.md); its internal structures are in
 * lib/gax/src/gax_internal.h. The API names are Shin'en's own (the
 * engine's error reports name GAX2_new/GAX2_init/GAX2_jingle/GAX_irq).
 *
 * Usage, as the game's music player does it: GAX2_new(params), fill in
 * the song (`layout`, `sfxTypes`) and work buffer, GAX2_init(params);
 * then GAX_irq() every VBlank and GAX_play() once per frame. */

struct GaxHandlerType;

/* GAX2's parameter block: filled with defaults by
 * GAX2_new, sized by GAX2_estimate and handed to GAX2_init, which keeps
 * a pointer to it as the player state's `songPtr`. */
struct GaxSongHeader {
    u8 *workBuf;  /* 0x00 - caller-supplied work RAM */
    u32 workSize; /* 0x04 - its size (GAX2_estimate computes the requirement) */
    u16 mixRate;  /* 0x08 - 0xffff = the song's default */
    u16 filter; /* 0x0a - low-pass filter amount (0-0x55, 0 = off), copied to GaxPlayerState.filter each GAX_play */
    u16 flags;  /* 0x0c */
    u16 numSfx; /* 0x0e - number of SFX voices, 0xffff = the song's default */
    u16 volume; /* 0x10 - master volume, clamped to 0xff; 0xffff = 0xff */
    u8 pad_12[0x1a];
    struct GaxHandlerType **sfxTypes; /* 0x2c - handler types of the SFX voices, or NULL */
    /* 0x30 - the music player's handler layout; SFX voices follow its `count` handlers */
    struct GaxHandlerLayout *layout;
    void *scratch;  /* 0x34 - 0x40-byte buffer cleared every tick */
    u8 showErrors;  /* 0x38 - show GAX2's fatal-error screen on failure */
    u8 songEnded;   /* 0x39 - the current player's song has ended (GaxInfoHandler.songEnded) */
    u8 jingleEnded; /* 0x3a - set when a finished jingle hands back to the music player */
};

/* A player's handler layout: `count` handler types, instantiated in
 * order into the player's handler array (GaxCreateHandlers). */
struct GaxHandlerLayout {
    u32 count;
    struct GaxHandlerType *types[1]; /* really `count` long */
};

/* Setup */
void GAX2_new(void *params);
void GAX2_estimate(struct GaxSongHeader *params);
u8 GAX2_init(struct GaxSongHeader *params);
u32 GAX2_jingle(struct GaxHandlerLayout *layout);

/* Per frame: GAX_irq from the VBlank handler, GAX_play to mix. */
void GAX_irq(void);
void GAX_play(void);

/* Music */
void GAX_pause(void);
void GAX_resume(void);
void GAX_stop(void);
void GAX_set_music_volume(s32 channel, u32 volume);

/* Sound effects */
s32 GAX_fx(u32 instrument);
s32 GAX_fx_ex(u32 instrument, s32 channel, s32 priority, s32 pitch);
void GAX_fx_note(s32 channel, u32 period);
void GAX_stop_fx(s32 channel);
void GAX_set_fx_volume(s32 channel, u32 volume);

#endif /* __GAX_H__ */
