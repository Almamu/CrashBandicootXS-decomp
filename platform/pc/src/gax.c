/* GAX2's API (lib/gax/include/gax.h), which the pc platform provides in
 * place of the library (docs/platforms.md, "What a port provides"):
 * stubs, silent. A mixer that plays the built music and sound-effect
 * data is phase 6 of #1093. */

#include <gax.h>

void GAX2_new(void *params)
{
    (void)params;
}

void GAX2_estimate(struct GaxSongHeader *params)
{
    (void)params;
}

u8 GAX2_init(struct GaxSongHeader *params)
{
    (void)params;
    return 1;
}

u32 GAX2_jingle(struct GaxHandlerLayout *layout)
{
    (void)layout;
    return 0;
}

void GAX_irq(void)
{
}

void GAX_play(void)
{
}

void GAX_pause(void)
{
}

void GAX_resume(void)
{
}

void GAX_stop(void)
{
}

void GAX_set_music_volume(s32 channel, u32 volume)
{
    (void)channel;
    (void)volume;
}

s32 GAX_fx(u32 instrument)
{
    (void)instrument;
    return -1;
}

s32 GAX_fx_ex(u32 instrument, s32 channel, s32 priority, s32 pitch)
{
    (void)instrument;
    (void)priority;
    (void)pitch;
    return channel;
}

void GAX_fx_note(s32 channel, u32 period)
{
    (void)channel;
    (void)period;
}

void GAX_stop_fx(s32 channel)
{
    (void)channel;
}

void GAX_set_fx_volume(s32 channel, u32 volume)
{
    (void)channel;
    (void)volume;
}
