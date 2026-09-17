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
    u32 slotId;      /* GAX2 instrument/sample handle - 0 = unused slot */
    u32 chanArg;       /* passed through to sub_8038E74's channel-select arg (only PlaySfx reads this; sub_80019F8 hardcodes 0) */
    u32 baseVolume;      /* multiplied by the caller's volume param and AudioContext.sfxVolume, then >>16 */
};

extern struct SfxTableEntry gStaticData_0816AA6C[99];

/* The GAX2 engine's own runtime player-state object - `gUnknown_03001630`
 * is an IWRAM *pointer variable* holding this struct's address (set up by
 * `sub_8038538`, the still-raw play-start/init entry point - see
 * docs/audio.md). Field layout is only partly understood; only the
 * fields this pass's small handful of matched functions actually touch
 * are named here - everything else stays unmodeled. `channels[]`'s
 * length (2) is fixed by `curChannelIdx` sitting at +0x10 in every
 * function that reads it. */
struct GaxPlayerState {
    u32 magic;             /* 0x00 - 0x47415832 ("GAX2") once a song is loaded */
    void *songPtr;          /* 0x04 - the struct passed as sub_8038538's arg0 */
    void *channels[2];        /* 0x08 */
    u32 curChannelIdx;          /* 0x10 */
    u8 pad_14[0x1c];               /* 0x14-0x2f - not modeled by this pass */
    u32 state;                       /* 0x30 - 0 = stopped, 1 = starting, 2 = playing */
};

#endif /* __AUDIO_H__ */
