#ifndef GUARD_PC_GAX_HOST_H
#define GUARD_PC_GAX_HOST_H

/* The pc platform's GAX2 (docs/platforms.md, "Audio"): what its units of
 * lib/gax (platform/pc/units/lib/gax/src/), the generated song data
 * (tools/gax_host_data.py) and the audio output (audio.c) share. The
 * engine is lib/gax's matched C; the units replace only what can't run on
 * a host: the ARM DSP routines (gax_dsp.c), the self-modifying resampler
 * patches, the work buffer's carving by the GBA's sizes, the DMA and
 * SWI asm. */

#include "gax_internal.h"

/* The song data as the host's structures (gax_host_data.c, generated):
 * the converted layout of the music block's song at `offset`, or, for the
 * entry with `types`, the sound-effect voice types AudioContext hands
 * GAX2_init as gGaxMusicData (its first words). */
struct HostGaxLayout {
    u32 offset;
    struct GaxHandlerLayout *layout;
    struct GaxHandlerType **types;
};

extern const struct HostGaxLayout gHostGaxLayouts[];
extern const u32 gHostGaxLayoutCount;

/* A layout the game names by its GBA address in the music block: the
 * converted one (gax_playstart.c). Any other pointer is returned as it
 * is. */
struct GaxHandlerLayout *HostGaxLayout(struct GaxHandlerLayout *layout);

/* What a handler array's slot 2 holds: a handler of no type
 * (gax_playstart.c). */
extern struct GaxHandler gHostGaxNoHandler;

/* The ARM routines of lib/gax/asm/gax_arm_dsp.s, in C (gax_dsp.c), with
 * the same work items GAX2's Thumb code builds for them. */

/* gGaxArmResample's item (GaxChannelMix's struct GaxMixItem). */
struct HostGaxMixItem {
    u8 *src;
    void *buf;
    s32 pos;
    s32 end;
    u32 frames;
    volatile u32 done;
    u32 volume;
    u32 step;
    u32 mode;
    s32 loopLen;
};

/* The resampler, as GaxChannelMix's patches of its copy left it: the
 * store and add loops each step forward (`add`, `blt`) or backward
 * (`sub`, `bgt`). */
void HostGaxResample(struct HostGaxMixItem *item, int storeBackward, int mixBackward);
void HostGaxDownmix(const s16 *src, s8 *dst, u32 samples, u32 gain);
/* The echo and the filter keep state in their copy of the code
 * (.Lecho_pos, .Lfilter_state), which GAX2_init copies afresh:
 * HostGaxDspReset clears it. */
void HostGaxEcho(u32 bytes, s16 *buf, s16 *echoBuf, u32 echoLen, const u32 *taps);
void HostGaxFilter(s16 *buf, u32 bytes, u32 amount, u32 channels);
void HostGaxDspReset(void);

/* The audio output (audio.c): GaxMixFrame's 8-bit block, `samples` at
 * `rate` Hz, as DMA1 would play it to Direct Sound A. */
void HostAudioMixed(const s8 *buf, u32 samples, u32 rate);

#endif /* GUARD_PC_GAX_HOST_H */
