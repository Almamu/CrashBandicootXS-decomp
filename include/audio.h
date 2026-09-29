#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "core.h"

/* The music/SFX-trigger "context" object `PlaySfx` and its neighbors take
 * as their first argument - `*gUnknown_030012BC` in the ROM, an
 * 8340-byte allocation made by `sub_8022230` (see docs/rom_map.md's
 * "Found the origin point" section). Only the leading 0x58 bytes this
 * cluster of functions models by field are covered here; starting at
 * +0x58 sits an embedded GAX2 runtime player-state object (initialized by
 * `sub_80381FC`/`sub_8038538`, both still raw engine internals) that
 * `sub_80017BC` pokes directly - genuinely nested, not-yet-reverse-
 * engineered state, so those writes stay raw offset casts (see
 * docs/audio.md) rather than guessed struct fields. `sub_8001B14` also
 * pokes one more field (+0x62) inside that same embedded region for the
 * same reason.
 *
 * Two independent fade-envelope pairs are tracked, each ramping by a
 * fixed +-0x10 (Q8.8, ~0.06) per `sub_80016EC` tick once its direction
 * flag is armed:
 *   - `musicVolCurrent`/`musicVolTarget` (+0x18/+0x1c, hardware-mirrored
 *     as a u16 at +0x68 - just past this struct) - independent of the
 *     ducking pair below.
 *   - `duckVolCurrent`/`duckVolTarget` (+0x24/+0x28) - the music-ducking
 *     ramp `sub_8001AC4` (duck out, explicit target)/`sub_8001AD8` (duck
 *     back in, restores `duckVolDefault`) drive; `duckVolDefault` (+0x20)
 *     is the last value explicitly set via `sub_8001B30`.
 *
 * A second pair of 3-word records tracks a currently-playing/queued
 * "ambient" sound effect (distinct from the one-shot `PlaySfx` calls):
 * `activeSfx`/`pendingSfx`, each `{id, gUnknown_0300082C-relative
 * deadline, volume}` - see `sub_800190C`/`sub_80019CC`/`sub_80019E8`/
 * `sub_80019F8`. `0x63` (99) is the "none" sentinel for both ids.
 * `sub_800190C` copies `pendingSfx` over `activeSfx` as a single 12-byte
 * struct assignment (not 3 separate field copies) - that's what gets it
 * to reproduce the ROM's `ldm/stm {r2,r3,r5}` block-move codegen. */
struct SfxRecord {
    u32 id;              // 0x63 (99) = none
    u32 deadline;          // gUnknown_0300082C-relative
    s32 volume;              // target the owning field_34 fade-timer ramps toward
};

struct AudioContext {
    u32 field_00;         // 0x00 - not touched by this function cluster
    u32 state;             // 0x04 - 0 = stopped, 1 = playing, 2 = paused
    u32 currentSong;        // 0x08 - index into gStaticData_0816AA20 (19 songs); 0x13 = none
    u32 pendingSong;         // 0x0c - queued song index, started once the duck-out fade completes
    u32 lastSfxId[2];         // 0x10/0x14 - round-robin record of the last 2 PlaySfx ids (sub_80019A8 stop-if-playing scan)
    s32 musicVolCurrent;       // 0x18 - signed: sub_80016EC compares it with blt/bgt, not an unsigned bcc/bcs
    s32 musicVolTarget;         // 0x1c
    s32 duckVolDefault;          // 0x20
    s32 duckVolCurrent;           // 0x24
    s32 duckVolTarget;             // 0x28
    s32 sfxVolume;                  // 0x2c - PlaySfx's own volume multiplier
    u32 field_30;                    // 0x30 - mirrored (truncated) into the embedded GAX object's +0xA (self+0x62) while playing
    s32 field_34;                     // 0x34 - the ambient-sfx channel's own current fade volume (ramps toward activeSfx.volume, sub_800190C - signed, compared with bgt/bge/ble)
    struct SfxRecord activeSfx;         // 0x38
    struct SfxRecord pendingSfx;          // 0x44
    u8 musicVolFadeUpArmed;                       // 0x50
    u8 musicVolFadeDownArmed;                      // 0x51
    u8 duckVolFadeUpArmed;                           // 0x52
    u8 duckVolFadeDownArmed;                          // 0x53
    u8 field_54;                                        // 0x54 - only ever cleared in this cluster (sub_80017BC, on a successful song start)
    u8 pad_55[3];                                         // 0x55-0x57
};

COMPILE_TIME_ASSERT(sizeof(struct AudioContext) == 0x58);

/* One record of the 99-entry sound-effect trigger table at ROM
 * `0x0816AA6C` (`sound/sfx_table.json`) - see docs/audio.md's "Sound
 * effects" section. Indexed by the id `PlaySfx`/`sub_80019F8` take. */
struct SfxTableEntry {
    u32 slotId;      /* instrument index into the sound-effect data set (gStaticData_084C0006, docs/audio.md) - 0 = unused slot */
    u32 chanArg;       /* passed through as sub_8038E74's priority arg (only PlaySfx reads this; sub_80019F8 hardcodes 0) */
    u32 baseVolume;      /* multiplied by the caller's volume param and AudioContext.sfxVolume, then >>16 */
};

extern struct SfxTableEntry gStaticData_0816AA6C[99];

/* The GAX2 engine's own runtime player-state object - `gUnknown_03001630`
 * is an IWRAM *pointer variable* holding this struct's address (carved
 * out of the caller's work RAM by `sub_8038538`, the play-start/init
 * entry point - see docs/audio.md). Field layout is only partly
 * understood; only the fields matched functions touch are named.
 * `channels[]` holds the two players' handler arrays (0 = music, 1 =
 * sound effects - see GAX_PLAYER() below), selected by
 * `curChannelIdx`. */
struct GaxPlayerState {
    u32 magic;             /* 0x00 - 0x47415832 ("GAX2") once a song is loaded */
    void *songPtr;          /* 0x04 - the struct passed as sub_8038538's arg0 */
    void *channels[2];        /* 0x08 */
    u32 curChannelIdx;          /* 0x10 */
    struct GaxChannelFormat *format; /* 0x14 - the output format (sub_8038538) */
    u32 field_18;                    /* 0x18 - base of the mix output buffer pair */
    u32 field_1c;                    /* 0x1c */
    u32 field_20;                    /* 0x20 - copied into every DSP work item (gax_unknownc_play.c) */
    u32 field_24;                    /* 0x24 - ditto */
    u32 field_28;                    /* 0x28 - ditto */
    u32 field_2c;                    /* 0x2c */
    u32 state;                       /* 0x30 - 0 = stopped, 1 = starting, 2 = playing */
    u32 field_34;                    /* 0x34 - from the mix-rate table */
    u8 pad_38[8];                    /* 0x38-0x3f - not modeled yet */
    u8 field_40;                     /* 0x40 */
    u8 field_41;                     /* 0x41 */
    u8 field_42;                     /* 0x42 */
    u8 field_43;                     /* 0x43 - set once a mixer tick ran */
    void *field_44;                  /* 0x44 - ARM callback, entered via GAX_CALL_ARM */
    /* 0x48/0x9c - IWRAM copies of the ARM DSP routines raw at
     * 0x0803A628/0x0803A67C (see gax_unknownc_play.c), entered in place
     * via GAX_CALL_ARM. */
    u32 dspCode48[21];               /* 0x48 */
    u32 dspCode9c[56];               /* 0x9c */
    void *dspFn17c;                  /* 0x17c - optional ARM routine, entered via GAX_CALL_ARM */
    u32 field_180;                   /* 0x180 - clamped to <= 0x55 each tick */
    u8 *workBuf;                     /* 0x184 - the caller-supplied work buffer channel tables are carved from */
    u32 workSize;                    /* 0x188 */
};

/* ---- GAX2 sound handlers (docs/audio.md's GAX2_SoundHandler types) ----
 * Only fields matched code touches are named; the rest stay padding. */

/* A volume envelope: `count` breakpoints, optional sustain point and
 * loop range (0xff = none). */
struct GaxEnvelopePoint {
    u16 pos;                     /* tick of this breakpoint */
    s16 slope;                   /* Q8 value change per tick up to the next one */
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
    u8 field_00;                 /* 0x00 - nonzero = sweep enabled */
    u8 pingPong;                 /* 0x01 - sweep bounces instead of wrapping */
    u8 pad_02[2];
    s32 start;                   /* 0x04 - sample start position */
    s32 sweepMin;               /* 0x08 - lower bound of the ping-pong sweep */
    s32 sweepMax;                /* 0x0c - upper bound */
    s32 sweepLen;                /* 0x10 */
    s32 sweepStep;               /* 0x14 */
    u16 sweepRate;               /* 0x18 - ticks between sweep steps */
    s16 tune;                    /* 0x1a */
};

/* One step of an instrument's sequence (sub_80398DC): an optional note
 * and wave-row change plus two effect commands (`cmd << 8 | param`). */
struct GaxInstrumentSeqEntry {
    u8 note;                     /* 0 = none */
    u8 field_01;
    u8 wave;                     /* 1-based rows[] index, 0 = none */
    u8 pad_03;
    u16 fx[2];
};

struct GaxChannelInstrument {
    u8 pad_00;
    u8 waveIdx[4];               /* 0x01 - per-row wave index */
    u8 pad_05[4];
    u8 vibratoDepth;             /* 0x09 - Q8 scale of the vibrato table value, 0 = off (sub_8039FFC) */
    u8 vibratoSpeed;             /* 0x0a - phase step per tick */
    u8 pad_0b;
    struct GaxInstrumentRow rows[4]; /* 0x0c */
    struct GaxEnvelope *envelope; /* 0x7c */
    u8 pad_80[5];
    u8 seqLen;                   /* 0x85 */
    u8 pad_86[2];
    struct GaxInstrumentSeqEntry *seq; /* 0x88 - per-tick instrument sequence */
};

/* The song data an Info handler's type points at. */
/* One sample. */
struct GaxWave {
    u8 *data;                    /* NULL = empty slot */
    u32 length;
};

struct GaxSongData {
    u8 pad_00[8];
    u16 volume;                  /* 0x08 - Q8 master volume */
    u8 pad_0a[2];
    u8 *patterns;                /* 0x0c - base of the packed pattern streams */
    u8 pad_10[4];
    struct GaxWave *waves;       /* 0x14 */
    u16 mixRate;                 /* 0x18 - default mix rate */
    u8 numSfx;                   /* 0x1a - default number of SFX voices */
    u8 field_1b;                 /* 0x1b */
};

/* One entry of a Channel handler type's order list. */
struct GaxOrderEntry {
    u16 patternOffset;           /* this channel's pattern for the order, relative to song->patterns */
    s8 transpose;                /* in semitones */
    u8 pad_03;
};

/* An ARM routine's tap/rate table (the mixer type's data). */
struct GaxDspTap {
    u32 value;                   /* +0 */
    u32 rate;                    /* +4 */
};

struct GaxDspParams {
    struct GaxDspTap taps[4];
};

/* A handler type: the three per-type callbacks plus type data. */
struct GaxHandlerType {
    void (*init)(void *handler);                         /* 0x00 */
    void (*unknown)(void *handler);                      /* 0x04 */
    u8 (*play)(void *handler, void *buf, u32 arg);       /* 0x08 */
    u32 childCount;                                      /* 0x0c */
    struct GaxHandlerType **childTypes;                  /* 0x10 - type of each child, NULL = none */
    u32 instanceSize;                                    /* 0x14 - bytes after the 12-byte GaxHandler header */
    union {
        struct GaxSongData *song;    /* Info handlers */
        struct GaxOrderEntry *orders; /* Channel handlers */
        struct GaxDspParams *dsp;     /* the mixer */
    } data;                                              /* 0x18 */
};

/* Every handler starts with this 12-byte header (sub_8038240 carves
 * `instanceSize` more bytes, then the children array, after it). */
struct GaxHandler {
    struct GaxHandlerType *type;       /* 0x00 */
    struct GaxChannelFormat *format;   /* 0x04 */
    struct GaxHandler **children;      /* 0x08 */
};

/* The shared "Info" handler every Channel handler's children[0] is. */
struct GaxInfoHandler {
    struct GaxHandlerType *type; /* 0x00 */
    u8 pad_04[0x10];
    s16 orderPos;                /* 0x14 */
    u8 pad_16[2];
    u16 speed;                   /* 0x18 - ticks per row */
    u8 field_1a;                 /* 0x1a */
    u8 field_1b;                 /* 0x1b */
    u8 tickCounter;              /* 0x1c */
    u8 field_1d;                 /* 0x1d */
    u8 newOrder;                 /* 0x1e - set when the order position changed */
    u8 field_1f;                 /* 0x1f */
    u8 field_20;                 /* 0x20 */
    u8 field_21;                 /* 0x21 */
    u8 field_22;                 /* 0x22 */
    u8 pad_23;
    u16 field_24;                /* 0x24 */
};

/* The top-level mixer handler, `handlers[0]` of a GAX2 player (the
 * "UnknownC" type, see gax_unknownc_play.c): its children are the
 * song's channels, followed by `extraChildren` sound-effect voices. */
struct GaxMixerFormat {
    u8 pad_00;
    u8 channels;                  /* 0x01 - 1 = mono, 2 = stereo */
    u8 pad_02[2];
    u16 frames;                   /* 0x04 - samples per mix buffer */
};

struct GaxMixerHandler {
    struct GaxHandlerType *type;  /* 0x00 */
    struct GaxMixerFormat *format; /* 0x04 */
    struct GaxHandler **children; /* 0x08 */
    u32 pos;                      /* 0x0c */
    u32 field_10;                 /* 0x10 */
    u32 extraChildren;            /* 0x14 - number of SFX voices after the song's channels */
};

/* The object `gUnknown_03001630->songPtr` points at. */
struct GaxSongHeader {
    u8 *workBuf;                 /* 0x00 - caller-supplied work RAM */
    u32 workSize;                /* 0x04 - its size (sub_8037FC0 computes the requirement) */
    u16 mixRate;                 /* 0x08 - 0xffff = the song's default */
    u16 field_0a;              /* 0x0a - copied to GaxPlayerState.field_180 each tick */
    u16 flags;                   /* 0x0c */
    u16 numSfx;                  /* 0x0e - number of SFX voices, 0xffff = the song's default */
    u16 field_10;                /* 0x10 - clamped to 0xff; 0xffff = 0xff */
    u8 pad_12[0x1a];
    struct GaxHandlerType **sfxTypes; /* 0x2c - handler types of the SFX voices, or NULL */
    struct GaxHandlerLayout *layout; /* 0x30 - the music player's handler layout; SFX voices follow its `count` handlers */
    void *scratch;               /* 0x34 - 0x40-byte buffer cleared every tick */
    u8 showErrors;               /* 0x38 - show GAX2's fatal-error screen on failure */
    u8 field_39;                 /* 0x39 */
    u8 field_3a;                 /* 0x3a */
};

/* A player's handler layout: `count` handler types, instantiated in
 * order into the player's handler array (sub_8038240). */
struct GaxHandlerLayout {
    u32 count;
    struct GaxHandlerType *types[1]; /* really `count` long */
};

extern struct GaxPlayerState *gUnknown_03001630;

/* Typed views of the current player: its handler array (`[0]` = mixer,
 * `[1]` = the shared Info handler, then the song's channels) and song
 * header. `channels[]`/`songPtr` stay `void *` in GaxPlayerState and
 * every access re-derives the chain, as the ROM does; the element types
 * matter - they're what lets gcc's type-based alias analysis keep a
 * loaded `songPtr` in a register across `children[]` stores. */
#define GAX_PLAYER() ((struct GaxHandler **)gUnknown_03001630->channels[gUnknown_03001630->curChannelIdx])
#define GAX_SONG() ((struct GaxSongHeader *)gUnknown_03001630->songPtr)
#define GAX_MIXER() ((struct GaxMixerHandler *)GAX_PLAYER()[0])
#define GAX_INFO() ((struct GaxInfoHandler *)GAX_PLAYER()[1])

/* The output format every handler's `format` points at (built by
 * sub_8038538 right after the player's handler array). */
struct GaxChannelFormat {
    u8 field_00;                 /* 0x00 - 8 */
    u8 channels;                 /* 0x01 - 1 */
    u16 mixRate;                 /* 0x02 - Hz */
    u16 frames;                  /* 0x04 - samples per video frame (mixRate * 1000 / 59727) */
};

/* A "Channel" handler: one tracker channel's playback state (the `self`
 * of sub_8039518/sub_80395A4/sub_8039658/sub_8039AA4/sub_8039F30/
 * sub_803A03C, ...). */
struct GaxChannelState {
    struct GaxHandlerType *type; /* 0x00 */
    struct GaxChannelFormat *format; /* 0x04 */
    struct GaxHandler **children; /* 0x08 - [0] is the GaxInfoHandler */
    u8 field_0c;                 /* 0x0c */
    u8 field_0d;                 /* 0x0d */
    u8 emptyPattern;             /* 0x0e - first byte of the pattern: nonzero = nothing to decode */
    u8 rowSkip;                  /* 0x0f - rows left to skip in the pattern stream */
    u8 row;                      /* 0x10 - instrument->rows[] index, 0-3 */
    s8 field_11;                 /* 0x11 */
    u8 sweepOn;                  /* 0x12 */
    s8 sweepDir;                 /* 0x13 - +1/-1 */
    u8 sweepTimer;               /* 0x14 */
    u8 vol15;                    /* 0x15 - 0-0xff, ramped by volStep15 */
    u8 envOut;                   /* 0x16 - sub_8039F30's result */
    u8 vol17;                    /* 0x17 - 0-0xff, ramped by volStep17 */
    s8 field_18;                 /* 0x18 - volume set by sub_8039064/sub_80390F8 (-1 = default) */
    u8 pad_19;
    s16 volStep15;               /* 0x1a */
    s16 volStep17;               /* 0x1c */
    u8 cutDelay;                 /* 0x1e */
    u8 cutTimer;                 /* 0x1f */
    u8 seqLoopCount;             /* 0x20 - sequence loop counter (cmds 5/6) */
    u8 field_21;                 /* 0x21 */
    u8 released;                /* 0x22 - nonzero once the note is released (envelope leaves sustain/loop) */
    u8 vibratoDelay;             /* 0x23 - ticks left before the vibrato phase starts advancing */
    u8 field_24;                 /* 0x24 - muted flag (sub_8038FD0) */
    u8 field_25;                 /* 0x25 */
    s16 pitch;                   /* 0x26 */
    s16 pitchStep;               /* 0x28 */
    s16 note;                    /* 0x2a - 0x8AD0 = no note */
    s16 noteStep;                /* 0x2c */
    s16 field_2e;                /* 0x2e - added to the note when mixing (sub_8039FFC's vibrato offset) */
    s16 slideTarget;             /* 0x30 */
    s16 slideRate;               /* 0x32 - 0 = no portamento */
    s16 retriggerDelay;          /* 0x34 - E-Dx note delay countdown */
    u16 seqPos;                  /* 0x36 - position in instrument->seq */
    u16 envPos;                 /* 0x38 - sub_8039F30's position */
    u16 vibratoPhase;            /* 0x3a - 0-0x3f index into gStaticData_085A9EAC */
    struct GaxChannelInstrument *instrument; /* 0x3c */
    u8 *patternPtr;              /* 0x40 - read position in the packed pattern stream */
    s32 samplePos;               /* 0x44 - Q11 */
    s32 sweepPos;                /* 0x48 */
    s32 priority;                /* 0x4c - voice-steal priority; INT_MIN (0x80000000) on note-off */
    u8 delayedNote;              /* 0x50 - note/instrument held for a delayed retrigger */
    u8 delayedInstrument;        /* 0x51 */
    u8 field_52;                 /* 0x52 */
    u8 index;                    /* 0x53 - channel number (sub_8038240) */
};

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
 * instead of a stack reload) - sub_8039B44's form, taking the player
 * state whose `field_44` holds the routine. What reproduces the ROM
 * (docs/matching/gax-naked-retry-3.md):
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
        _fn = ((struct GaxPlayerState *)_fn)->field_44;                          \
        asm volatile("mov r1, %1\n\tmov r0, %0\n\tmov r2, pc\n\tadd r2, #5\n\t" \
                     "mov lr, r2\n\tbx r1\n\tnop"                               \
                     : : "r"(_arg), "r"(_fn) : "r0", "r1", "r2", "lr", "memory"); \
    }

#endif /* __AUDIO_H__ */
