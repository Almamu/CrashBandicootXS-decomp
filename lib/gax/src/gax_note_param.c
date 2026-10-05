#include "gax_internal.h"

extern struct GaxPlayerState *gGaxPlayerState;

/* Sets the pitch of sound-effect voice `channel` of the current player,
 * if that voice has an instrument loaded. The SFX voices are the mixer's
 * children after the song's own channels (`type->childCount` of them),
 * so the voice is `mixer->children[type->childCount + channel]`. */
void GAX_fx_note(s32 channel, u32 period)
{
    struct GaxPlayerState *p;
    struct GaxHandler **handlers;
    struct GaxMixerHandler *mixer;
    struct GaxHandlerType *type;
    struct GaxChannelState *voice;

    if (period <= 0xeec) {
        p = gGaxPlayerState;
        handlers = p->channels[p->curChannelIdx];
        mixer = (struct GaxMixerHandler *)handlers[0];
        type = mixer->type;
        voice = (struct GaxChannelState *)mixer->children[type->childCount + channel];
        if (voice->instrument != NULL) {
            voice->pitch = period;
        }
    }
}
asm(".align 2, 0");
