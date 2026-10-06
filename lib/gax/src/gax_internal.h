#ifndef __GAX_INTERNAL_H__
#define __GAX_INTERNAL_H__

#include "gba/gba.h"
#include <gax.h>

/* GAX2's internal structures (the engine's own player state, handler
 * types and instances, song data), shared by lib/gax's objects. Not part
 * of the public interface (<gax.h>). */

/* The GAX2 engine's own runtime player-state object - `gGaxPlayerState`
 * is an IWRAM *pointer variable* holding this struct's address (carved
 * out of the caller's work RAM by `GAX2_init`, the play-start/init
 * entry point - see docs/audio.md). Field layout is only partly
 * understood; only the fields matched functions touch are named.
 * `channels[]` holds the two players' handler arrays (0 = music, 1 =
 * the jingle player `GAX2_jingle` builds - see GAX_PLAYER() below),
 * selected by `curChannelIdx`. */
struct GaxPlayerState {
    u32 magic;                       /* 0x00 - 0x47415832 ("GAX2") once a song is loaded */
    void *songPtr;                   /* 0x04 - the struct passed as GAX2_init's arg0 */
    void *channels[2];               /* 0x08 */
    u32 curChannelIdx;               /* 0x10 */
    struct GaxChannelFormat *format; /* 0x14 - the output format (GAX2_init) */
    /* 0x18 - the 8-bit output double buffer (2 x frames), DMA1's source */
    u32 outBuf;
    /* 0x1c - the 16-bit mix buffer ((frames + 4) halfwords) the mixer renders into */
    u32 mixBuf;
    /* 0x20 - the echo delay line, sized for the longest DSP tap delay */
    u32 echoBuf;
    /* 0x24 - the 3 echo taps' {step, value} pairs (GaxCreateHandlers), 0 = no echo */
    u32 echoTaps;
    u32 echoLen; /* 0x28 - echoBuf's size in bytes */
    /* 0x2c - which half of outBuf GAX_play mixes into next (0/1) */
    u32 outHalf;
    u32 state;       /* 0x30 - 0 = stopped, 1 = starting, 2 = playing */
    u32 timerReload; /* 0x34 - Timer0 reload for the mix rate (gGaxMixRates) */
    u8 pad_38[8];    /* 0x38-0x3f - not modeled yet */
    /* 0x40 - mix the SFX voices before the echo pass, so they get echo too */
    u8 fxEcho;
    /* 0x41 - set by GAX2_jingle; skips the song's channels for one mixer tick */
    u8 skipSongChannels;
    /* 0x42 - mixCode holds the whole resampler (76 words, with its third,
     * paired-sample loop) rather than the first 55: set when the song's
     * GaxSongData.field_1b or GaxSongHeader.flags bit 5 is (GAX2_init) */
    u8 fullResampler;
    /* 0x43 - GAX_play ran since the last GAX_irq ("GAX_PLAY HAS NOT FINISHED" check) */
    u8 playDone;
    void *mixCode; /* 0x44 - IWRAM copy of the ARM resampler gGaxArmResample (GaxChannelMix) */
    /* 0x48/0x9c/0x17c - IWRAM copies of the ARM routines gGaxArmDownmix,
     * gGaxArmEcho and (only with GaxSongHeader.flags bit 2, else NULL)
     * gGaxArmFilter (see gax_unknownc_play.c), entered in place via
     * GAX_CALL_ARM. */
    u32 downmixCode[21]; /* 0x48 */
    u32 echoCode[56];    /* 0x9c */
    void *filterCode;
    /* 0x180 - low-pass filter amount, GaxSongHeader.filter clamped to 0x55
     * (GaxMixerPlay); 0 = off */
    u32 filter;
    u8 *workBuf;  /* 0x184 - the caller-supplied work buffer channel tables are carved from */
    u32 workSize; /* 0x188 */
};

/* ---- GAX2 sound handlers (docs/audio.md's GAX2_SoundHandler types) ----
 * Only fields matched code touches are named; the rest stay padding. */

/* A volume envelope: `count` breakpoints, optional sustain point and
 * loop range (0xff = none). */
struct GaxEnvelopePoint {
    u16 pos;   /* tick of this breakpoint */
    s16 slope; /* Q8 value change per tick up to the next one */
    u8 value;
    u8 pad_05[3];
};

struct GaxEnvelope {
    u8 count;
    u8 sustain;
    u8 loopStart;
    u8 loopEnd;
    struct GaxEnvelopePoint points[1]; /* really `count` long */
};

/* One of an instrument's 4 wave rows (28 bytes). */
struct GaxInstrumentRow {
    u8 sweep;    /* 0x00 - nonzero = sweep mode; 0 = loop between sweepMin and sweepMax */
    u8 pingPong; /* 0x01 - sweep bounces instead of wrapping */
    u8 pad_02[2];
    s32 start;     /* 0x04 - sample start position */
    s32 sweepMin;  /* 0x08 - lower bound of the ping-pong sweep */
    s32 sweepMax;  /* 0x0c - upper bound */
    s32 sweepLen;  /* 0x10 */
    s32 sweepStep; /* 0x14 */
    u16 sweepRate; /* 0x18 - ticks between sweep steps */
    s16 tune;      /* 0x1a */
};

/* One step of an instrument's sequence (GaxChannelStepInstrumentSeq): an optional note
 * and wave-row change plus two effect commands (`cmd << 8 | param`). */
struct GaxInstrumentSeqEntry {
    u8 note;       /* 0 = none */
    u8 fixedPitch; /* the note ignores pitch slides and the order's transpose
                    * (tools/gax_audio.py's `dont_use_note_pitch`) */
    u8 wave;       /* 1-based rows[] index, 0 = none */
    u8 pad_03;
    u16 fx[2];
};

struct GaxChannelInstrument {
    u8 pad_00;
    u8 waveIdx[4]; /* 0x01 - per-row wave index */
    u8 pad_05[4];
    /* 0x09 - Q8 scale of the vibrato table value, 0 = off (GaxChannelTickVibrato) */
    u8 vibratoDepth;
    u8 vibratoSpeed; /* 0x0a - phase step per tick */
    u8 pad_0b;
    struct GaxInstrumentRow rows[4]; /* 0x0c */
    struct GaxEnvelope *envelope;    /* 0x7c */
    u8 pad_80[5];
    u8 seqLen; /* 0x85 */
    u8 pad_86[2];
    struct GaxInstrumentSeqEntry *seq; /* 0x88 - per-tick instrument sequence */
};

/* The song data an Info handler's type points at. */
/* One sample. */
struct GaxWave {
    u8 *data; /* NULL = empty slot */
    u32 length;
};

struct GaxSongData {
    u8 pad_00[2];
    u16 patternRows; /* 0x02 - rows per pattern */
    u16 orderCount;  /* 0x04 - entries in each channel's order list */
    u16 loopOrder;   /* 0x06 - order position the song loops back to */
    u16 volume;      /* 0x08 - Q8 master volume */
    u8 pad_0a[2];
    u8 *patterns; /* 0x0c - base of the packed pattern streams */
    u8 pad_10[4];
    struct GaxWave *waves; /* 0x14 */
    u16 mixRate;           /* 0x18 - default mix rate */
    u8 numSfx;             /* 0x1a - default number of SFX voices */
    u8 field_1b;           /* 0x1b */
};

/* One entry of a Channel handler type's order list. */
struct GaxOrderEntry {
    u16 patternOffset; /* this channel's pattern for the order, relative to song->patterns */
    s8 transpose;      /* in semitones */
    u8 pad_03;
};

/* An ARM routine's tap/rate table (the mixer type's data). */
struct GaxDspTap {
    u32 value; /* +0 */
    u32 rate;  /* +4 */
};

struct GaxDspParams {
    struct GaxDspTap taps[4];
};

/* A handler type: the three per-type callbacks plus type data. */
struct GaxHandlerType {
    void (*init)(void *handler);                   /* 0x00 */
    void (*unknown)(void *handler);                /* 0x04 */
    u8 (*play)(void *handler, void *buf, u32 arg); /* 0x08 */
    u32 childCount;                                /* 0x0c */
    struct GaxHandlerType **childTypes;            /* 0x10 - type of each child, NULL = none */
    /* 0x14 - bytes after the 12-byte GaxHandler header */
    u32 instanceSize;
    union {
        struct GaxSongData *song;     /* Info handlers */
        struct GaxOrderEntry *orders; /* Channel handlers */
        struct GaxDspParams *dsp;     /* the mixer */
    } data;                           /* 0x18 */
};

/* A song layout's `types[2]` slot holds a list of alternative layouts
 * (GAX2_estimate, GAX2_init; both files had a copy). */
struct GaxLayoutList {
    u32 count;
    struct GaxHandlerLayout *layouts[1];
};

/* Every handler starts with this 12-byte header (GaxCreateHandlers carves
 * `instanceSize` more bytes, then the children array, after it). */
struct GaxHandler {
    struct GaxHandlerType *type;     /* 0x00 */
    struct GaxChannelFormat *format; /* 0x04 */
    struct GaxHandler **children;    /* 0x08 */
};

/* The shared "Info" handler every Channel handler's children[0] is. */
struct GaxInfoHandler {
    struct GaxHandlerType *type;     /* 0x00 */
    struct GaxChannelFormat *format; /* 0x04 */
    struct GaxHandler **children;    /* 0x08 */
    u32 lastTick;                    /* 0x0c - the play_fn argument of the last tick run */
    u32 firstTick;                   /* 0x10 - the first one, 0 = none yet */
    s16 orderPos;                    /* 0x14 */
    s16 row;                         /* 0x16 - row in the current pattern */
    u16 speed;                       /* 0x18 - ticks per row; a nonzero high byte
                                      * alternates with the low byte every row */
    u8 playing;                      /* 0x1a - 0 = the song is stopped (no rows advance) */
    /* 0x1b - while nonzero (counted down each tick) every channel drops
     * its instrument: GaxInfoRestart's 2 silent ticks */
    u8 muteTicks;
    u8 tickCounter; /* 0x1c */
    u8 newRow;      /* 0x1d - set on the tick a new row starts */
    u8 newOrder;    /* 0x1e - set when the order position changed */
    /* 0x1f - master volume, from GaxSongHeader.volume each GAX_play (0xff = full) */
    u8 volume;
    /* 0x20 - stop instead of looping after the last order (jingles; GAX2_init flag bit 3) */
    u8 stopAtEnd;
    u8 songEnded; /* 0x21 - set once the song has played past its last order (or stopped) */
    /* 0x22 - pattern-break effect (cmd 13): jump to the pattern's end on the next row */
    u8 patternBreak;
    u8 pad_23;
    u16 breakRow; /* 0x24 - the pattern-break effect's (cmd 13) parameter; stored, never read */
};

/* The top-level mixer handler, `handlers[0]` of a GAX2 player (the
 * "UnknownC" type, see gax_unknownc_play.c): its children are the
 * song's channels, followed by `extraChildren` sound-effect voices. */
struct GaxMixerFormat {
    u8 pad_00;
    u8 channels; /* 0x01 - 1 = mono, 2 = stereo */
    u8 pad_02[2];
    u16 frames; /* 0x04 - samples per mix buffer */
};

struct GaxMixerHandler {
    struct GaxHandlerType *type;   /* 0x00 */
    struct GaxMixerFormat *format; /* 0x04 */
    struct GaxHandler **children;  /* 0x08 */
    u32 pos;                       /* 0x0c */
    /* 0x10 - GaxPlayerState.mixBuf, the buffer the mix renders into */
    u32 mixBuf;
    u32 extraChildren; /* 0x14 - number of SFX voices after the song's channels */
};


extern struct GaxPlayerState *gGaxPlayerState;

/* Typed views of the current player: its handler array (`[0]` = mixer,
 * `[1]` = the shared Info handler, then the song's channels) and song
 * header. `channels[]`/`songPtr` stay `void *` in GaxPlayerState and
 * every access re-derives the chain, as the ROM does; the element types
 * matter - they're what lets gcc's type-based alias analysis keep a
 * loaded `songPtr` in a register across `children[]` stores. */
#define GAX_PLAYER() ((struct GaxHandler **)gGaxPlayerState->channels[gGaxPlayerState->curChannelIdx])
#define GAX_SONG() ((struct GaxSongHeader *)gGaxPlayerState->songPtr)
#define GAX_MIXER() ((struct GaxMixerHandler *)GAX_PLAYER()[0])
#define GAX_INFO() ((struct GaxInfoHandler *)GAX_PLAYER()[1])

/* The output format every handler's `format` points at (built by
 * GAX2_init right after the player's handler array). */
struct GaxChannelFormat {
    u8 bits;     /* 0x00 - bits per output sample, 8 */
    u8 channels; /* 0x01 - 1 */
    u16 mixRate; /* 0x02 - Hz */
    u16 frames;  /* 0x04 - samples per video frame (mixRate * 1000 / 59727) */
};

/* A "Channel" handler: one tracker channel's playback state (the `self`
 * of GaxChannelInit/GaxChannelPlay/GaxChannelDecodeRow/GaxChannelTick/GaxEnvelopeTick/
 * GaxChannelTickSweep, ...). */
struct GaxChannelState {
    struct GaxHandlerType *type;     /* 0x00 */
    struct GaxChannelFormat *format; /* 0x04 */
    struct GaxHandler **children;    /* 0x08 - [0] is the GaxInfoHandler */
    u8 muted;                        /* 0x0c - nonzero: not mixed (GaxChannelPlay) */
    /* 0x0d - set by GaxMixerPlay when no channel before this one produced
     * output: GaxChannelMix then stores into the buffer instead of adding */
    u8 isFirst;
    /* 0x0e - first byte of the pattern: nonzero = nothing to decode */
    u8 emptyPattern;
    u8 rowSkip;    /* 0x0f - rows left to skip in the pattern stream */
    u8 row;        /* 0x10 - instrument->rows[] index, 0-3 */
    s8 direction;  /* 0x11 - > 0 forward, else backward (ping-pong loops flip it) */
    u8 sweepOn;    /* 0x12 */
    s8 sweepDir;   /* 0x13 - +1/-1 */
    u8 sweepTimer; /* 0x14 */
    u8 vol15;      /* 0x15 - 0-0xff, ramped by volStep15 */
    u8 envOut;     /* 0x16 - GaxEnvelopeTick's result */
    u8 vol17;      /* 0x17 - 0-0xff, ramped by volStep17 */
    /* 0x18 - volume set by GAX_set_music_volume/GAX_set_fx_volume (-1 = default) */
    s8 volume;
    u8 pad_19;
    s16 volStep15;   /* 0x1a */
    s16 volStep17;   /* 0x1c */
    u8 cutDelay;     /* 0x1e */
    u8 cutTimer;     /* 0x1f */
    u8 seqLoopCount; /* 0x20 - sequence loop counter (cmds 5/6) */
    u8 fixedPitch;   /* 0x21 - from the instrument sequence entry */
    /* 0x22 - nonzero once the note is released (envelope leaves sustain/loop) */
    u8 released;
    u8 vibratoDelay; /* 0x23 - ticks left before the vibrato phase starts advancing */
    /* 0x24 - SFX voices: note queued by GAX_fx_ex (1 = key off, GAX_stop_fx) */
    u8 pendingNote;
    u8 pendingInstrument; /* 0x25 - SFX voices: instrument (sound effect id) queued by GAX_fx_ex */
    s16 pitch;            /* 0x26 */
    s16 pitchStep;        /* 0x28 */
    s16 note;             /* 0x2a - 0x8AD0 = no note */
    s16 noteStep;         /* 0x2c */
    /* 0x2e - added to the note when mixing (GaxChannelTickVibrato's vibrato offset) */
    s16 vibratoOffset;
    s16 slideTarget;                         /* 0x30 */
    s16 slideRate;                           /* 0x32 - 0 = no portamento */
    s16 retriggerDelay;                      /* 0x34 - E-Dx note delay countdown */
    u16 seqPos;                              /* 0x36 - position in instrument->seq */
    u16 envPos;                              /* 0x38 - GaxEnvelopeTick's position */
    u16 vibratoPhase;                        /* 0x3a - 0-0x3f index into gGaxVibratoTable */
    struct GaxChannelInstrument *instrument; /* 0x3c */
    u8 *patternPtr;                          /* 0x40 - read position in the packed pattern stream */
    s32 samplePos;                           /* 0x44 - Q11 */
    s32 sweepPos;                            /* 0x48 */
    /* 0x4c - voice-steal priority; INT_MIN (0x80000000) on note-off */
    s32 priority;
    /* 0x50 - note/instrument held for a delayed retrigger */
    u8 delayedNote;
    u8 delayedInstrument; /* 0x51 */
    u8 mixMode;           /* 0x52 - the resampler mode used to add a forward sample, always 1 */
    u8 index;             /* 0x53 - channel number (GaxCreateHandlers) */
};

/* A mixing rate in Hz and the Timer0 reload for it (16.78 MHz / rate),
 * one entry of gGaxMixRates. */
struct RateEntry {
    u32 rate;
    u32 timer;
};

/* ---- The engine's functions shared between lib/gax's objects ---- */

/* gax_channel_bind_instrument.c */
extern void GaxChannelSetInstrument(struct GaxChannelState *self, struct GaxInfoHandler *info,
                                    u32 instrument, struct GaxSongData *song);
/* gax_channel_effect_table.c */
extern void GaxChannelTickVibrato(struct GaxChannelState *self);
/* gax_channel_envelope_tick.c */
extern void GaxChannelTick(struct GaxChannelState *self, struct GaxInfoHandler *info);
/* gax_channel_note_cut.c */
extern void GaxChannelSetNote(struct GaxChannelState *self, u32 note);
/* gax_channel_note_scheduler.c */
extern void GaxChannelStepInstrumentSeq(struct GaxChannelState *self, struct GaxInfoHandler *info);
/* gax_channel_pos_sweep.c */
extern void GaxChannelTickSweep(struct GaxChannelState *self);
/* gax_channel_table_alloc.c */
extern u8 GaxCreateHandlers(struct GaxHandlerLayout *layout, struct GaxHandlerType **sfx,
                            u32 numSfx, u8 **bufp, u32 *sizep);
/* gax_dma_stop.c */
extern void GaxStopDma(u32 dmaIdx);
/* gax_fatal_error.c: the halt screen, with the failing function's name
 * and the error (gGaxErrName* / gGaxErr*). Never returns. */
extern void GaxFatalError(const char *function, const char *message);
/* gax_find_mix_rate.c */
extern s32 GaxFindMixRate(u32 rate);
/* gax_hw_reset.c */
extern void GaxResetSoundHardware(void);
/* gax_note_lookup.c */
extern u8 GaxEnvelopeTick(struct GaxChannelState *self, struct GaxEnvelope *env, u16 *posp);
/* gax_note_trigger.c */
extern u32 GaxChannelMix(struct GaxChannelState *self, struct GaxInfoHandler *info, void *buf,
                         u32 arg, struct GaxSongData *song, u8 flag);
/* gax_swi.c */
extern void GaxHuffUnComp(void *src, void *dst);
/* gax_text_render.c */
extern void GaxDrawText(u32 col, u32 row, const char *str);
/* gax_unknownc_play.c */
extern void GaxMixFrame(struct GaxMixerHandler *mixer, u32 *buf);
/* gax_zero_fill.c */
extern void GaxZeroFill(void *dest, s32 count);

/* The SoundHandler types' methods (init_fn/unknown_fn/play_fn, see
 * docs/audio.md) and the helpers next to them. The methods aren't called
 * by name (the handler-type tables hold their addresses) and the helpers
 * only from their own file, but each definition still needs a prototype
 * (-Wmissing-prototypes). */
struct UnknownC; /* gax_unknownc_play.c's view of the mixer handler */
/* gax_channel_init.c, gax_channel_note_cut_driver.c: the FX channel type */
extern void GaxFxChannelInit(void *self);
extern u8 GaxFxChannelPlay(struct GaxChannelState *self, void *buf, u32 arg);
/* gax_sound_handler_channel.c, gax_sound_handler_channel_init.c,
 * gax_sound_handler_channel_play.c: the Channel type */
extern void GaxChannelUnknown(void);
extern void GaxChannelInit(struct GaxChannelState *self);
extern u8 GaxChannelPlay(struct GaxChannelState *self, void *buf, u32 arg);
/* gax_sound_handler_info.c: the Info type */
extern void GaxInfoResetPosition(void *self);
extern void GaxInfoInit(void *self);
extern void GaxInfoRestart(void *self);
extern void GaxInfoUnknown(void);
extern u32 GaxInfoPlay(void *self, u32 arg1, u32 chanArg);
/* gax_sound_handler_unknownc.c, gax_unknownc_play.c: the mixer
 * ("UnknownC") type */
extern void GaxFxChannelUnknown(void);
extern void GaxMixerInit(void *self);
extern void GaxMixerUnknown(void);
extern void GaxMixerApplyEcho(struct UnknownC *self, u32 *buf);
extern void GaxMixerApplyFilter(struct UnknownC *self, u32 *buf, u32 clampArg, u32 count);
extern u8 GaxMixerPlay(struct UnknownC *self, u32 *buf, u32 arg2);

/* ---- The engine's data ---- */

/* lib/gax/data/gax_tables_5a6100.c */
/* "GAX Sound Engine 2.01D ..." (GAX2_init checks "GAX") */
extern const char *const gGaxVersionStringPtr;
extern const struct RateEntry gGaxMixRates[12];
extern const char gGaxErrNameNew[]; /* the function names and errors GaxFatalError shows */
extern const char gGaxErrParamsNull[];
extern const char gGaxErrNameInit[];
extern const char gGaxErrOutOfMemory[];
extern const char gGaxErrNameJingle[];
extern const char gGaxErrNoJingle[];
extern const char gGaxErrNameIrq[];
extern const char gGaxErrPlayNotFinished[];
extern const char *const gGaxHaltBannerPtr;
extern const char gGaxHaltFunctionLabel[];
extern const u32 gGaxPeriodTable[0xEF4];
extern const s8 gGaxVibratoTable[64];

/* The raw ARM routines at the end of gax_unknownc_play.c, which GAX2_init
 * copies into the player state (downmixCode/echoCode/filterCode/mixCode),
 * and the four instructions of the resampler that GaxChannelMix patches
 * in the copy (by their offset from gGaxArmResample). */
extern const u32 gGaxArmDownmix[];
extern const u32 gGaxArmFilter[];
extern const u32 gGaxArmEcho[];
extern const u32 gGaxArmResample[];
extern const u32 gGaxArmResampleStoreStep[];
extern const u32 gGaxArmResampleStoreEndTest[];
extern const u32 gGaxArmResampleMixStep[];
extern const u32 gGaxArmResampleMixEndTest[];

/* data/data.s: the handler layout GAX2_init and GAX2_estimate use when the
 * song header names none. */
extern struct GaxHandlerLayout gGaxDefaultSong;

/* src/iwram/iwram_data.c: the halt screen's font, Huffman-compressed
 * (GaxFatalError decompresses it with GaxHuffUnComp). */
extern u32 gGaxHaltFont[70];

/* sym_iwram.txt (gGaxPlayerState is declared above) */
extern u64 gGaxMixRateReciprocal; /* 2^32 / mix rate (GaxChannelInit), scales gGaxPeriodTable */

/* GAX2's own "call an ARM routine from Thumb" idiom (ARMv4T Thumb has
 * no `blx reg`): hand-computes a Thumb-tagged return address into lr
 * and `bx`es to `fn` with `*argp` in r0, returning to the trailing
 * `nop`. The operand shapes (`"m"` for the argument, forcing it through
 * a stack slot, and a free register moved into r1) are what reproduce
 * the ROM's own `str r0,[sp,#N]` ... `mov r1,rX; ldr r0,[sp,#N]`
 * sequence, so this is very likely the engine's own inline asm. */
#define GAX_CALL_ARM(fn, arg)                                                    \
    asm volatile("mov r1, %1\n\tldr r0, %0\n\tmov r2, pc\n\tadd r2, #5\n\t"     \
                 "mov lr, r2\n\tbx r1\n\tnop"                                   \
                 : : "m"(arg), "r"(fn) : "r0", "r1", "r2", "lr")

/* The same call with the argument already in a register (`mov r0, rX`
 * instead of a stack reload) - GaxChannelMix's form, taking the player
 * state whose `mixCode` holds the routine. What reproduces the ROM
 * (docs/matching/archive/gax-naked-retry-3.md):
 * - the "memory" clobber: the ARM routine writes the work item `arg`
 *   points at, and without it GCSE carries loads across the call;
 * - `arg` goes into a register before the routine is loaded (the ROM's
 *   `mov r4, sp` comes first);
 * - one variable walks state -> routine, so both loads share a register
 *   (the ROM's `ldr r3, [r3]; ldr r3, [r3, #0x44]`). */
#define GAX_CALL_ARM_R(state, arg)                                               \
    {                                                                            \
        void *_arg = (void *)(arg);                                              \
        void *_fn = (state);                                                     \
        _fn = ((struct GaxPlayerState *)_fn)->mixCode;                          \
        asm volatile("mov r1, %1\n\tmov r0, %0\n\tmov r2, pc\n\tadd r2, #5\n\t" \
                     "mov lr, r2\n\tbx r1\n\tnop"                               \
                     : : "r"(_arg), "r"(_fn) : "r0", "r1", "r2", "lr", "memory"); \
    }

#endif /* __GAX_INTERNAL_H__ */
